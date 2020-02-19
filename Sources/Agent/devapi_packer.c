#include "devapi.h"
#include "devapi_packer.h"
#include "pl.h"
#include "dm.h"

/* WARNGING: not reentrant */
const void *
ptr_htobe(enum devapi_datatype type, const void *val)
{
	pl_assert(val != NULL);

	static uint8_t val_dup[8];

	switch (type) {
	case DEVAPI_DATATYPE_INT16:
	case DEVAPI_DATATYPE_UINT16:
		memcpy(val_dup, val, 2);
		*(int16_t*)val_dup = pl_htobe16(*(int16_t*)val_dup);
		return val_dup;
	case DEVAPI_DATATYPE_ENUM:
	case DEVAPI_DATATYPE_INT32:
	case DEVAPI_DATATYPE_UINT32:
	case DEVAPI_DATATYPE_FLOAT1:
	case DEVAPI_DATATYPE_FLOAT2:
	case DEVAPI_DATATYPE_FLOAT3:
	case DEVAPI_DATATYPE_FLOATE1:
	case DEVAPI_DATATYPE_FLOATE18:
		memcpy(val_dup, val, 4);
		*(int32_t*)val_dup = pl_htobe32(*(int32_t*)val_dup);
		return val_dup;
	case DEVAPI_DATATYPE_INT64:
	case DEVAPI_DATATYPE_UINT64:
		memcpy(val_dup, val, 8);
		*(int64_t*)val_dup = pl_htobe64(*(int64_t*)val_dup);
		return val_dup;
	default:
		return val;
	}
	return val;
}

static inline void
pack(struct devapi_msg_packer_ctx *ctx, const void *val, size_t val_len)
{
	pl_assert(ctx != NULL && val != NULL);
	
	memcpy(ctx->curr_buf_ptr, val, val_len);
	ctx->curr_buf_ptr += val_len;
	ctx->curr_buf_size += val_len;
}

static inline enum devapi_return
unpack(struct devapi_msg_packer_ctx *ctx, const char *param,
		void *val, size_t val_len)
{
	pl_assert(ctx != NULL && param != NULL && val != NULL);

	if (ctx->max_buf_size < ctx->curr_buf_size + val_len) {
		PL_LOG_ERROR("Unpack %s - no memory, has [%u], need [%u]", param,
			(ctx->max_buf_size - ctx->curr_buf_size), val_len);
		return DEVAPI_PACKER_NO_MEM;
	}
	memcpy(val, ctx->curr_buf_ptr, val_len);
	ctx->curr_buf_ptr += val_len;
	ctx->curr_buf_size += val_len;
	return DEVAPI_SUCCESS;
}

static inline enum devapi_return
unpack_str(struct devapi_msg_packer_ctx *ctx, const char *param,
		void *str, size_t str_max_len)  // str_max_len including '\0'
{
	pl_assert(ctx != NULL && param != NULL && str != NULL);

	size_t slen = strlen(ctx->curr_buf_ptr) + 1;  // XXX: possible buffer overflow read
	if (slen > str_max_len) {
		PL_LOG_ERROR("Received %s's length is [%u], expected < [%d]",
				param, slen, str_max_len);
		return DEVAPI_PACKER_NO_MEM;
	}
	return unpack(ctx, param, str, slen);
}

static inline void
adjust_msg_hdr_size(struct devapi_msg_packer_ctx *ctx) {
	pl_assert(ctx != NULL);
	// Re-adjust the message size in the header
	uint16_t new_len = pl_htobe16(ctx->curr_buf_size);
	memcpy(ctx->hdr_ptr, &new_len, sizeof(new_len));
}

void
devapi_msg_packer_init(struct devapi_msg_packer_ctx *ctx,
		void *buf, size_t buf_size)
{
	pl_assert(ctx != NULL);
	ctx->hdr_done = 0;
	ctx->hdr_ptr = buf;
	ctx->curr_buf_ptr = buf;
	ctx->obj_start_ptr = NULL;
	ctx->param_start_ptr = NULL;
	ctx->curr_buf_size = 0;
	ctx->max_buf_size = buf_size;
	PL_LOG_DEBUG("Packer ctx initialized");
}

enum devapi_return
devapi_msg_pack_msghdr(struct devapi_msg_packer_ctx *ctx,
		uint32_t msg_id,
		enum devapi_msg_type msg_type,
		enum devapi_schema_type target_schema,
		const char *schema_id)
{
	pl_assert (ctx != NULL && schema_id != NULL);
	// Fill the header
	struct devapi_msg_hdr hdr;
	uint16_t hdr_len = 2 + 4 + 1 + 1 + 1 + 1 + (strlen(schema_id) + 1);

	if (ctx->max_buf_size < hdr_len) {
		PL_LOG_ERROR("Bufsize[%u] not enough for msg_hdr [%u]",
				ctx->max_buf_size, hdr_len);
		return DEVAPI_PACKER_NO_MEM;
	}

	hdr.msg_len = pl_htobe16(hdr_len);
	hdr.msg_id = pl_htobe32(msg_id);
	hdr.msg_type = msg_type;
	hdr.target_schema = target_schema;
	hdr.id_notation = DEVAPI_NOTATION_XPATH;	// Default: XPATH
	hdr.msg_last_fragment = 1;

	ctx->hdr_ptr = ctx->curr_buf_ptr;

	pack(ctx, (void*)&hdr.msg_len, sizeof(hdr.msg_len));
	pack(ctx, (void*)&hdr.msg_id, sizeof(hdr.msg_id));
	pack(ctx, (void*)&hdr.msg_type, sizeof(hdr.msg_type));
	pack(ctx, (void*)&hdr.target_schema, sizeof(hdr.target_schema));
	pack(ctx, (void*)&hdr.id_notation, sizeof(hdr.id_notation));
	pack(ctx, (void*)&hdr.msg_last_fragment, sizeof(hdr.msg_last_fragment));
	pack(ctx, schema_id, strlen(schema_id) + 1);

	ctx->hdr_done = 1;

	PL_LOG_DEBUG("Header packed. Len[%d]", hdr_len);
	return DEVAPI_SUCCESS;
}

enum devapi_return
devapi_msg_pack_ptv(struct devapi_msg_packer_ctx *ctx,
		const char *name,
		enum devapi_datatype datatype,
		const void *value)
{
	pl_assert(ctx != NULL && name != NULL && value != NULL);

	size_t name_len, type_len, val_len;
	bool is_str = false;

	name_len = strlen(name) + 1; // Include null-term
	type_len = sizeof(uint8_t);

	if (dm_get_datatype_size((uint8_t)datatype, &val_len, &is_str) != 0) {
		PL_LOG_ERROR("Invalid datatype: %d", datatype);
		return DEVAPI_INVALID_PARAM;
	}
	if (is_str) {
		// We pack strings tight
		val_len = strlen((const char *)value) + 1;
	}

	if (ctx->max_buf_size <
			(ctx->curr_buf_size + name_len + type_len + val_len)) {
		PL_LOG_ERROR("Bufsize[%u - %u] not enough for TLV [%u + %u + %u]",
				ctx->max_buf_size, ctx->curr_buf_size,
				name_len, type_len, val_len);
		return DEVAPI_PACKER_NO_MEM;
	}

	pack(ctx, name, name_len);  // Add the name
	pack(ctx, &datatype, type_len);  // Add datatype
	pack(ctx, ptr_htobe(datatype, value), val_len);  // Add value
	adjust_msg_hdr_size(ctx);  // Re-adjust the message size in the header

	PL_LOG_DEBUG("Name[%s] datatype[%s] val with length [%d] added",
		name, devapi_datatype_str(datatype), val_len);
	return DEVAPI_SUCCESS;
}

enum devapi_return
devapi_msg_pack_string(struct devapi_msg_packer_ctx *ctx, const char *xpath)
{
	pl_assert(ctx != NULL && xpath != NULL);

	uint32_t len = strlen(xpath) + 1;

	if (ctx->max_buf_size < (ctx->curr_buf_size + len)) {
		PL_LOG_ERROR("Bufsize[%u - %u] not enough to add XPATH[%u]",
				ctx->max_buf_size, ctx->curr_buf_size, len);
		return DEVAPI_PACKER_NO_MEM;
	}

	pack(ctx, xpath, len);
	adjust_msg_hdr_size(ctx);  // Re-adjust the message size in the header

	PL_LOG_DEBUG("Added XPATH[%s]", xpath);
	return DEVAPI_SUCCESS;
}

enum devapi_return
devapi_msg_pack_obj(struct devapi_msg_packer_ctx *ctx, const char *xpath)
{
	pl_assert(ctx != NULL && xpath != NULL);

	/*
	 * Adds the following 3 only if it is the first object (!obj_start_ptr):
	 * string: "num_of_objs"
	 * uint8_t: datatype = DEVAPI_DATATYPE_UINT8
	 * uint16_t: number of objects = 1
	 * The following are always added
	 * string: xpath
	 * string: "num_of_params"
	 * uint8_t: datatype = DEVAPI_DATATYPE_UINT8
	 * uint16_t: number of parameters = 0
	 */
	uint16_t len = strlen(xpath) + 1;
	len += strlen(S_NUM_OF_PARAMS) + 1 + sizeof(uint8_t) + sizeof(uint16_t);

	if (ctx->obj_start_ptr == NULL) {  // Needs to add object header too
		len += strlen(S_NUM_OF_OBJS) + 1 +
				sizeof(uint8_t) + sizeof(uint16_t);
	}

	if ((ctx->curr_buf_size + len) > ctx->max_buf_size) {
		PL_LOG_ERROR("Bufsize[%u - %u] not enough to add obj[%u]",
				ctx->max_buf_size, ctx->curr_buf_size, len);
		return DEVAPI_PACKER_NO_MEM;
	}
	if (ctx->obj_start_ptr == NULL) {  // There is no object added
		// Add object header
		ctx->obj_start_ptr = ctx->curr_buf_ptr;
		uint16_t nobjs = 1;
		devapi_msg_pack_ptv(ctx, S_NUM_OF_OBJS, DEVAPI_DATATYPE_UINT16,
				&nobjs);
		PL_LOG_DEBUG("Added first object with header");
	} else {  // This is not the first object
		// Increment num_of_objs existing in the message
		uint8_t *ptr = ctx->obj_start_ptr + strlen(S_NUM_OF_OBJS)+1 + 1;
		*(uint16_t *)ptr = pl_htobe16(1 + pl_be16toh(*(uint16_t *)ptr));
		PL_LOG_DEBUG("Object count incremented");
	}

	// Add object xpath
	devapi_msg_pack_string(ctx, xpath);

	// Add param header for this object
	ctx->param_start_ptr = ctx->curr_buf_ptr;

	uint16_t cnt = 0;
	devapi_msg_pack_ptv(ctx, S_NUM_OF_PARAMS, DEVAPI_DATATYPE_UINT16, &cnt);

	PL_LOG_DEBUG("Current buffer size[%u]", ctx->curr_buf_size);
	return DEVAPI_SUCCESS;
}

enum devapi_return
devapi_msg_pack_obj_param(struct devapi_msg_packer_ctx *ctx,
		const char *param_name,
		enum devapi_datatype param_type,
		const void *val)
{
	pl_assert(ctx != NULL && param_name != NULL);
	pl_assert(ctx->obj_start_ptr != NULL);  // Object header should be there

	uint32_t len = strlen(param_name) + 1;
	if (val) {
		size_t val_len = devapi_get_val_len(param_type, val);
		if (val_len == 0) {
			return DEVAPI_INVALID_PARAM;
		}
		// needs to pack datatype and value
		len += sizeof(uint8_t) + val_len;
	}

	if ((ctx->curr_buf_size + len) > ctx->max_buf_size) {
		PL_LOG_ERROR("Bufsize[%u - %u] not enough to add param[%u]",
				ctx->max_buf_size, ctx->curr_buf_size, len);
		return DEVAPI_PACKER_NO_MEM;
	}

	if (val == NULL) {
		// Add only param_name
		devapi_msg_pack_string(ctx, param_name);
	} else {
		// Add param name, type and value
		devapi_msg_pack_ptv(ctx, param_name, param_type, val);
	}

	// Increment num_of_params existing in the message
	uint8_t *ptr = ctx->param_start_ptr + strlen(S_NUM_OF_PARAMS)+1 + 1;
	*(uint16_t *)ptr = pl_htobe16(1 + pl_be16toh(*(uint16_t *)ptr));

	PL_LOG_DEBUG("Current buffer size[%u]", ctx->curr_buf_size);
	return DEVAPI_SUCCESS;
}

uint16_t
devapi_msg_get_num_objs(const struct devapi_msg_packer_ctx *ctx) {
	pl_assert(ctx != NULL);

	if (ctx->obj_start_ptr == NULL) {
		return 0;
	}
	uint8_t *ptr = ctx->obj_start_ptr + strlen(S_NUM_OF_OBJS)+1 + 1;
	return pl_be16toh(*(uint16_t *)ptr);
}

enum devapi_return
devapi_msg_pack_status(struct devapi_msg_packer_ctx *ctx,
		enum devapi_rsp_status status,
		const char *err_msg)
{
	pl_assert(ctx != NULL);

	// Add the status
	uint32_t slen = sizeof(uint8_t);
	uint32_t elen = 0;
	if (err_msg) {
		elen = strlen(err_msg) + 1;
	}

	if (ctx->max_buf_size < (ctx->curr_buf_size + slen + elen)) {
		PL_LOG_ERROR("Bufsize[%u - %u] not enough to pack status[%u]",
				ctx->max_buf_size, ctx->curr_buf_size,
				(slen+elen));
		return DEVAPI_PACKER_NO_MEM;
	}

	uint8_t status8 = status;
	pack(ctx, &status8, sizeof(status8));  // pack status
	if (err_msg) {
		pack(ctx, err_msg, elen);  // pack error_msg
	}
	adjust_msg_hdr_size(ctx);  // Re-adjust the message size in the header

	PL_LOG_DEBUG("Current buffer size[%u]", ctx->curr_buf_size);
	return DEVAPI_SUCCESS;
}

void
devapi_msg_mark_fragment(struct devapi_msg_packer_ctx *ctx)
{
	pl_assert(ctx != NULL && ctx->hdr_ptr != NULL);

	((struct devapi_msg_hdr *)(ctx->hdr_ptr))->msg_last_fragment = 0;
}


enum devapi_return
devapi_msg_unpack_msghdr(struct devapi_msg_packer_ctx *ctx,
		struct devapi_msg_hdr *hdr, const char **schema_id)
{
	pl_assert(ctx != NULL && hdr != NULL && schema_id != NULL);
	enum devapi_return ret;

	memset(hdr, 0, sizeof(*hdr));

	if (((ret = unpack(ctx, "msg_len", (void*)&hdr->msg_len,
			sizeof(hdr->msg_len))) == DEVAPI_SUCCESS) &&
		((ret = unpack(ctx, "msg_id", (void*)&hdr->msg_id,
				sizeof(hdr->msg_id))) == DEVAPI_SUCCESS) &&
		((ret = unpack(ctx, "msg_type", (void*)&hdr->msg_type,
				sizeof(hdr->msg_type))) == DEVAPI_SUCCESS) &&
		((ret = unpack(ctx, "target_schema", (void*)&hdr->target_schema,
				sizeof(hdr->target_schema))) == DEVAPI_SUCCESS) &&
		((ret = unpack(ctx, "id_notation", (void*)&hdr->id_notation,
				sizeof(hdr->id_notation))) == DEVAPI_SUCCESS) &&
		((ret = unpack(ctx, "msg_last_fragment", (void*)&hdr->msg_last_fragment,
				sizeof(hdr->msg_last_fragment))) == DEVAPI_SUCCESS)) {
		// set schema_id and adjust pointers
		*schema_id = ctx->curr_buf_ptr;
		size_t schema_id_len = strlen(ctx->curr_buf_ptr) + 1;  // XXX: possible buffer overflow on read
		if (ctx->max_buf_size < ctx->curr_buf_size + schema_id_len) {
			PL_LOG_ERROR("Unpack schema_id - no memory,"
				" has [%u], need [%u]",
				(ctx->max_buf_size - ctx->curr_buf_size),
				schema_id_len);
			return DEVAPI_PACKER_NO_MEM;
		}
		ctx->curr_buf_ptr += schema_id_len;
		ctx->curr_buf_size += schema_id_len;

		ctx->hdr_done = 1;
		// convert network to host order
		hdr->msg_len = pl_be16toh(hdr->msg_len);
		hdr->msg_id = pl_be32toh(hdr->msg_id);
		PL_LOG_DEBUG("Unpacked header len[%u], id[%u], type[%s], "
				"last_fragement?[%d], schema[%s]",
				hdr->msg_len, hdr->msg_id,
				devapi_msg_type_str(hdr->msg_type),
				hdr->msg_last_fragment, *schema_id);
	}
	return ret;
}

enum devapi_return
devapi_msg_unpack_status(struct devapi_msg_packer_ctx *ctx,
		struct devapi_req_result *result)
{
	pl_assert(ctx != NULL && result != NULL);
	pl_assert(ctx->hdr_done == 1);

	enum devapi_return ret;
	uint8_t rsp_status;

	// Read the status
	ret = unpack(ctx, "status", &rsp_status, sizeof(rsp_status));
	if (ret != DEVAPI_SUCCESS) {
		return ret;
	}
	result->status = rsp_status;
	if (result->status != DEVAPI_STATUS_SUCCESS) {
		// Read the error_msg
		ret = unpack_str(ctx, "error_msg", result->error_msg,
				DEVAPI_ERR_MSG_LEN);
		if (ret != DEVAPI_SUCCESS) {
			return ret;
		}
	}

	PL_LOG_DEBUG("Unpacked status successfully");
	return DEVAPI_SUCCESS;
}

enum devapi_return
devapi_msg_unpack_ptv(struct devapi_msg_packer_ctx *ctx,
		char *name, size_t name_len,
		enum devapi_datatype *datatype,
		void *val, size_t arg_val_len)
{
	pl_assert(ctx != NULL && name != NULL && datatype != NULL && val != NULL);
	enum devapi_return ret;
	uint8_t type;
	bool is_str;
	size_t val_len;

	// Extract name
	ret = unpack_str(ctx, "PTV-name", name, name_len);
	if (ret != DEVAPI_SUCCESS)
		return ret;
	// Extract type
	ret = unpack(ctx, "PTV-type", &type, sizeof(type));
	if (ret != DEVAPI_SUCCESS)
		return ret;
	*datatype = type;
	// Extract value
	if (dm_get_datatype_size(type, &val_len, &is_str) != 0) {
		PL_LOG_ERROR("Invalid datatype: %d", *datatype);
		return DEVAPI_INVALID_PARAM;
	}
	if (arg_val_len < val_len) {
		PL_LOG_ERROR("Valsize[%u] not enough for type[%u] val_len[%u]",
				arg_val_len, type, val_len);
		return DEVAPI_INVALID_PARAM;
	}
	if (is_str) {
		ret = unpack_str(ctx, "PTV-value", val, val_len);
	} else {
		ret = unpack(ctx, "PTV-value", val, val_len);
	}
	if (ret != DEVAPI_SUCCESS)
		return ret;
	if (!is_str) {
		PL_LOG_DEBUG("Extracted name[%s], type[%s], len[%u]",
				name, devapi_datatype_str(*datatype),
				val_len);
		// XXX: crude simplification to decide byte-order conversion
		if (val_len == 2) {
			*(uint16_t*)val = pl_be16toh(*(uint16_t*)val);
			//TODO: %d format specifier not the right one.
			PL_LOG_DEBUG("Extracted val[%d]", *(uint16_t*)val);
		} else if (val_len == 4) {
			*(uint32_t*)val = pl_be32toh(*(uint32_t*)val);
			PL_LOG_DEBUG("Extracted val[%d]", *(uint32_t*)val);
		} else if (val_len == 8) {
			*(uint64_t*)val = pl_be64toh(*(uint64_t*)val);
			PL_LOG_DEBUG("Extracted val[%d]", *(uint64_t*)val);
		}
	} else {
		PL_LOG_DEBUG("Extracted name[%s], type[%s], len[%u], val[%s]",
				name, devapi_datatype_str(*datatype),
				val_len, val);
	}
	return DEVAPI_SUCCESS;
}

enum devapi_return
devapi_msg_unpack_ptv_val(struct devapi_msg_packer_ctx *ctx,
		const char *name, enum devapi_datatype datatype, void *val)
{
	pl_assert(ctx != NULL && name != NULL && val != NULL);

	enum devapi_return ret;
	uint8_t type;
	char *param = ctx->curr_buf_ptr;
	size_t param_len = strlen(ctx->curr_buf_ptr) + 1;  // XXX: possible buffer overflow on read
	
	if (ctx->max_buf_size < ctx->curr_buf_size + param_len) {
		PL_LOG_ERROR("Unpack PTV-Param %s: no memory,"
			" has [%u], need [%u]", name,
			(ctx->max_buf_size - ctx->curr_buf_size),
			param_len);
		return DEVAPI_PACKER_NO_MEM;
	}
	ctx->curr_buf_ptr += param_len;
	ctx->curr_buf_size += param_len;

	if (strcmp(name, param) != 0) {
		PL_LOG_ERROR("PTV-Param %s: Found %s", name, param);
		return DEVAPI_INVALID_PARAM;
	}

	ret = unpack(ctx, "datatype", &type, sizeof(type));
	if (ret != DEVAPI_SUCCESS) {
		return ret;
	}
	if (type != datatype) {
		PL_LOG_ERROR("PTV-Type %s: Found %d, expected %d",
				name, type, datatype);
		return DEVAPI_INVALID_PARAM;
	}

	return devapi_msg_unpack_field(ctx, name, datatype, val);
}

enum devapi_return
devapi_msg_unpack_field(struct devapi_msg_packer_ctx *ctx,
		const char *field,
		enum devapi_datatype datatype,
		void *val)
{
	pl_assert(field != NULL && val != NULL);

	enum devapi_return ret;
	size_t val_len;
	bool is_str;

	// Extract value
	if (dm_get_datatype_size((uint8_t)datatype, &val_len, &is_str) != 0) {
		PL_LOG_ERROR("Invalid datatype: %d", datatype);
		return DEVAPI_INVALID_PARAM;
	}
	if (is_str) {
		ret = unpack_str(ctx, field, val, val_len);
	} else {
		ret = unpack(ctx, field, val, val_len);
	}
	if (ret != DEVAPI_SUCCESS) {
		return ret;
	}
	if (!is_str) {
		// XXX: crude simplification to decide byte-order conversion
		if (val_len == 2) {
			*(uint16_t*)val = pl_be16toh(*(uint16_t*)val);
		} else if (val_len == 4) {
			*(uint32_t*)val = pl_be32toh(*(uint32_t*)val);
		} else if (val_len == 8) {
			*(uint64_t*)val = pl_be64toh(*(uint64_t*)val);
		}
	}
	PL_LOG_DEBUG("Extracted %s - len [%u], val [%s]",
			field, val_len, (is_str ? val : ""));
	return DEVAPI_SUCCESS;
}

int
devapi_get_val_len(enum devapi_datatype datatype, const void *val)
{
	uint32_t ret = 0;

	switch (datatype) {
	case DEVAPI_DATATYPE_BOOL:
	case DEVAPI_DATATYPE_INT8:
	case DEVAPI_DATATYPE_UINT8:
		ret = sizeof(uint8_t);
		break;
	case DEVAPI_DATATYPE_INT16:
	case DEVAPI_DATATYPE_UINT16:
		ret = sizeof(uint16_t);
		break;
	case DEVAPI_DATATYPE_ENUM:
	case DEVAPI_DATATYPE_INT32:
	case DEVAPI_DATATYPE_UINT32:
		ret = sizeof(uint32_t);
		break;
	case DEVAPI_DATATYPE_INT64:
	case DEVAPI_DATATYPE_UINT64:
		ret = sizeof(uint64_t);
		break;
	case DEVAPI_DATATYPE_FLOAT1:
	case DEVAPI_DATATYPE_FLOAT2:
	case DEVAPI_DATATYPE_FLOAT3:
	case DEVAPI_DATATYPE_FLOATE1:
	case DEVAPI_DATATYPE_FLOATE18:
		ret = sizeof(float);  // 4 bytes?
		pl_assert(ret == 4);
		break;
	case DEVAPI_DATATYPE_STRING16:
	case DEVAPI_DATATYPE_STRING32:
	case DEVAPI_DATATYPE_STRING64:
	case DEVAPI_DATATYPE_STRING128:
	case DEVAPI_DATATYPE_STRING256:
		pl_assert(val != NULL);
		ret = strlen(val) + 1;
		break;
	case DEVAPI_DATATYPE_INVALID:
	default:
		ret = 0;
	}
	PL_LOG_DEBUG("Length[%u] type[%s]", ret, devapi_datatype_str(datatype));
	return ret;
}

const char *
devapi_return_str(enum devapi_return ret)
{
	switch (ret) {
		case DEVAPI_SUCCESS:
			return "DEVAPI_SUCCESS";
		case DEVAPI_INVALID_PARAM:
			return "DEVAPI_INVALID_PARAM";
		case DEVAPI_INVALID_MSGTYPE:
			return "DEVAPI_INVALID_MSGTYPE";
		case DEVAPI_DATATYPE_MISMATCH:
			return "DEVAPI_DATATYPE_MISMATCH";
		case DEVAPI_MEMORY_FAILURE:
			return "DEVAPI_MEMORY_FAILURE";
		case DEVAPI_FAILURE:
			return "DEVAPI_FAILURE";
		case DEVAPI_PACKER_NO_INIT:
			return "DEVAPI_PACKER_NO_INIT";
		case DEVAPI_PACKER_NO_HEADER:
			return "DEVAPI_PACKER_NO_HEADER";
		case DEVAPI_PACKER_NO_OBJ_HDR:
			return "DEVAPI_PACKER_NO_OBJ_HDR";
		case DEVAPI_PACKER_NO_MEM:
			return "DEVAPI_PACKER_NO_MEM";
		case DEVAPI_PACKER_FAILURE:
			return "DEVAPI_PACKER_FAILURE";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_rsp_status_str(enum devapi_rsp_status rsp)
{
	switch (rsp) {
		case DEVAPI_STATUS_SUCCESS:
			return "DEVAPI_STATUS_SUCCESS";
		case DEVAPI_STATUS_MSG_FORMAT_ERR:
			return "DEVAPI_STATUS_MSG_FORMAT_ERR";
		case DEVAPI_STATUS_REQ_UNSUPPORTED:
			return "DEVAPI_STATUS_REQ_UNSUPPORTED";
		case DEVAPI_STATUS_SCHEMA_ERR:
			return "DEVAPI_STATUS_SCHEMA_ERR";
		case DEVAPI_STATUS_APP_ERR:
			return "DEVAPI_STATUS_APP_ERR";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_sw_update_status_str(enum devapi_sw_update_status stat)
{
	switch (stat) {
		case DEVAPI_SU_STATUS_IDLE:
			return "DEVAPI_SU_STATUS_IDLE";
		case DEVAPI_SU_STATUS_DOWNLOAD_FAILED:
			return "DEVAPI_SU_STATUS_DOWNLOAD_FAILED";
		case DEVAPI_SU_STATUS_DOWNLOAD_SUCCESS:
			return "DEVAPI_SU_STATUS_DOWNLOAD_SUCCESS";
		case DEVAPI_SU_STATUS_UPDATE_FAILED:
			return "DEVAPI_SU_STATUS_UPDATE_FAILED";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_msg_type_str(enum devapi_msg_type msg)
{
	switch (msg) {
		case DEVAPI_MSG_INVALID:
			return "DEVAPI_MSG_INVALID";
		case DEVAPI_UNKNOWN_MSG:
			return "DEVAPI_UNKNOWN_MSG";
		case DEVAPI_HELLO_REQ:
			return "DEVAPI_HELLO_REQ";
		case DEVAPI_HELLO_RESP:
			return "DEVAPI_HELLO_RESP";
		case DEVAPI_GET_SCHEMA_REQ:
			return "DEVAPI_GET_SCHEMA_REQ";
		case DEVAPI_GET_SCHEMA_RESP:
			return "DEVAPI_GET_SCHEMA_RESP";
		case DEVAPI_GET_PARAM_REQ:
			return "DEVAPI_GET_PARAM_REQ";
		case DEVAPI_GET_PARAM_RESP:
			return "DEVAPI_GET_PARAM_RESP";
		case DEVAPI_GET_UPDATED_PARAM_REQ:
			return "DEVAPI_GET_UPDATED_PARAM_REQ";
		case DEVAPI_GET_UPDATED_PARAM_RESP:
			return "DEVAPI_GET_UPDATED_PARAM_RESP";
		case DEVAPI_TX_START_REQ:
			return "DEVAPI_TX_START_REQ";
		case DEVAPI_TX_START_RESP:
			return "DEVAPI_TX_START_RESP";
		case DEVAPI_SET_CONFIG_REQ:
			return "DEVAPI_SET_CONFIG_REQ";
		case DEVAPI_SET_CONFIG_RESP:
			return "DEVAPI_SET_CONFIG_RESP";
		case DEVAPI_TX_END_REQ:
			return "DEVAPI_TX_END_REQ";
		case DEVAPI_TX_END_RESP:
			return "DEVAPI_TX_END_RESP";
		case DEVAPI_REBOOT_REQ:
			return "DEVAPI_REBOOT_REQ";
		case DEVAPI_REBOOT_RESP:
			return "DEVAPI_REBOOT_RESP";
		case DEVAPI_RESET_REQ:
			return "DEVAPI_RESET_REQ";
		case DEVAPI_RESET_RESP:
			return "DEVAPI_RESET_RESP";
		case DEVAPI_SW_IMAGE_DOWNLOAD_REQ:
			return "DEVAPI_SW_IMAGE_DOWNLOAD_REQ";
		case DEVAPI_SW_IMAGE_DOWNLOAD_RESP:
			return "DEVAPI_SW_IMAGE_DOWNLOAD_RESP";
		case DEVAPI_SW_UPDATE_REQ:
			return "DEVAPI_SW_UPDATE_REQ";
		case DEVAPI_SW_UPDATE_RESP:
			return "DEVAPI_SW_UPDATE_RESP";
		case DEVAPI_GET_SW_UPDATE_STATUS_REQ:
			return "DEVAPI_GET_SW_UPDATE_STATUS_REQ";
		case DEVAPI_GET_SW_UPDATE_STATUS_RESP:
			return "DEVAPI_GET_SW_UPDATE_STATUS_RESP";
		case DEVAPI_MSG_MAX:
			return "DEVAPI_MSG_MAX";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_datatype_str(enum devapi_datatype type)
{
	switch (type) {
		case DEVAPI_DATATYPE_INVALID:
			return "DEVAPI_DATATYPE_INVALID";
		case DEVAPI_DATATYPE_BOOL:
			return "DEVAPI_DATATYPE_BOOL";
		case DEVAPI_DATATYPE_ENUM:
			return "DEVAPI_DATATYPE_ENUM";
		case DEVAPI_DATATYPE_INT8:
			return "DEVAPI_DATATYPE_INT8";
		case DEVAPI_DATATYPE_UINT8:
			return "DEVAPI_DATATYPE_UINT8";
		case DEVAPI_DATATYPE_INT16:
			return "DEVAPI_DATATYPE_INT16";
		case DEVAPI_DATATYPE_UINT16:
			return "DEVAPI_DATATYPE_UINT16";
		case DEVAPI_DATATYPE_INT32:
			return "DEVAPI_DATATYPE_INT32";
		case DEVAPI_DATATYPE_UINT32:
			return "DEVAPI_DATATYPE_UINT32";
		case DEVAPI_DATATYPE_INT64:
			return "DEVAPI_DATATYPE_INT64";
		case DEVAPI_DATATYPE_UINT64:
			return "DEVAPI_DATATYPE_UINT64";
		case DEVAPI_DATATYPE_FLOAT1:
			return "DEVAPI_DATATYPE_FLOAT1";
		case DEVAPI_DATATYPE_FLOAT2:
			return "DEVAPI_DATATYPE_FLOAT2";
		case DEVAPI_DATATYPE_FLOAT3:
			return "DEVAPI_DATATYPE_FLOAT3";
		case DEVAPI_DATATYPE_FLOATE1:
			return "DEVAPI_DATATYPE_FLOATE1";
		case DEVAPI_DATATYPE_FLOATE18:
			return "DEVAPI_DATATYPE_FLOATE18";
		case DEVAPI_DATATYPE_STRING16:
			return "DEVAPI_DATATYPE_STRING16";
		case DEVAPI_DATATYPE_STRING32:
			return "DEVAPI_DATATYPE_STRING32";
		case DEVAPI_DATATYPE_STRING64:
			return "DEVAPI_DATATYPE_STRING64";
		case DEVAPI_DATATYPE_STRING128:
			return "DEVAPI_DATATYPE_STRING128";
		case DEVAPI_DATATYPE_STRING256:
			return "DEVAPI_DATATYPE_STRING256";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_param_category_str(enum devapi_param_category pc)
{
	switch (pc) {
		case DEVAPI_PARAMCAT_INVALID:
			return "DEVAPI_PARAMCAT_INVALID";
		case DEVAPI_PARAMCAT_INVENTORY:
			return "DEVAPI_PARAMCAT_INVENTORY";
		case DEVAPI_PARAMCAT_CONFIG:
			return "DEVAPI_PARAMCAT_CONFIG";
		case DEVAPI_PARAMCAT_MONITOR:
			return "DEVAPI_PARAMCAT_MONITOR";
		case DEVAPI_PARAMCAT_FAULT:
			return "DEVAPI_PARAMCAT_FAULT";
		case DEVAPI_PARAMCAT_PERFORMANCE:
			return "DEVAPI_PARAMCAT_PERFORMANCE";
		case DEVAPI_PARAMCAT_ATTRIBUTE:
			return "DEVAPI_PARAMCAT_ATTRIBUTE";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_commit_opt_str(enum devapi_commit_opt co)
{
	switch (co) {
		case DEVAPI_COMMIT:
			return "DEVAPI_COMMIT";
		case DEVAPI_ROLLBACK:
			return "DEVAPI_ROLLBACK";
		default:
			return "UNKNOWN";
	}
}

const char *
devapi_card_type_str(enum devapi_card_type cardtype)
{
	switch (cardtype) {
		case DEVAPI_HALF_SLOT:
			return "DEVAPI_HALF_SLOT";
		case DEVAPI_FULL_SLOT:
			return "DEVAPI_FULL_SLOT";
		case DEVAPI_DOUBLE_SLOT:
			return "DEVAPI_DOUBLE_SLOT";
		default:
			return "UNKNOWN";
	}
}
