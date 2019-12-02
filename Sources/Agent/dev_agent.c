//#include <assert.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <fcntl.h>
#include "endian.h"
#include <stdbool.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

#include "devapi.h"
#include "devapi_packer.h"
#include "dev_agent_backend.h"
#include "dev_agent_conf.h"
#include "dev_agent_log.h"
#include "ospl.h"
#include "dm.h"
#include "dm_link.h"

int listen_sock = -1;

/*
 * devapi_msg_hdr is used when only hdr is populated.
 * agt_msg is used when it has payload also populated.
 */
struct agt_msg {
	struct devapi_msg_hdr hdr;
};

struct agt_buf {
	char buf[AGT_MAX_MSGLEN];
	size_t filled;		// size of req filled in the buf so far
	// In case of large messages filled can be more than AGT_MAX_MSGLEN,
	// but buffer overflow will not be there.
};

static struct agt_buf g_req_buf;
static struct agt_buf g_rsp_buf;

static void
init_buf(struct agt_buf *r_buf)
{
	memset(r_buf, 0, sizeof(*r_buf));
}

static void
dump_msg_hdr(const struct devapi_msg_hdr *hdr, const char *schema_id)
{
	LOG_VERBOSE("msg_type=%d, msg_id=%d, msg_len=%d, target_schema=%d,"
			" id_notation=%d, fragment=%d, schema_id=%s",
			hdr->msg_type, be32toh(hdr->msg_id),
			be16toh(hdr->msg_len),
			hdr->target_schema, hdr->id_notation,
			hdr->msg_last_fragment,
			(schema_id) ? schema_id : "");
}

static bool
is_req_msg_complete(const struct agt_buf *buf)
{
	const struct devapi_msg_hdr *hdr = (struct devapi_msg_hdr *)buf->buf;
	return ((buf->filled >= sizeof(hdr->msg_len)) &&
			(buf->filled == be16toh(hdr->msg_len)));
}

static bool
is_valid_request_type(enum devapi_msg_type type)
{
	switch (type) {
	case DEVAPI_HELLO_REQ:
	case DEVAPI_GET_SCHEMA_REQ:
	case DEVAPI_GET_PARAM_REQ:
	//case DEVAPI_GET_UPDATED_PARAM_REQ:
	//case DEVAPI_TX_START_REQ:
	//case DEVAPI_SET_CONFIG_REQ:
	//case DEVAPI_TX_END_REQ:
	//case DEVAPI_REBOOT_REQ:
	//case DEVAPI_RESET_REQ:
	//case DEVAPI_SW_IMAGE_DOWNLOAD_REQ:
	//case DEVAPI_SW_UPDATE_REQ:
	//case DEVAPI_GET_SW_UPDATE_STATUS_REQ:
		return true;
	default:
		return false;
	}
}

/*
 * Returns response message type for the given request message type.
 * If request message type is unknown, then returns DEVAPI_MSG_INVALID.
 */
static enum devapi_msg_type
get_resp_type(enum devapi_msg_type req_type)
{
	switch (req_type) {
	case DEVAPI_HELLO_REQ:
		return DEVAPI_HELLO_RESP;
	case DEVAPI_GET_SCHEMA_REQ:
		return DEVAPI_GET_SCHEMA_RESP;
	case DEVAPI_GET_PARAM_REQ:
		return DEVAPI_GET_PARAM_RESP;
	case DEVAPI_GET_UPDATED_PARAM_REQ:
		return DEVAPI_GET_UPDATED_PARAM_RESP;
	case DEVAPI_TX_START_REQ:
		return DEVAPI_TX_START_RESP;
	case DEVAPI_SET_CONFIG_REQ:
		return DEVAPI_SET_CONFIG_RESP;
	case DEVAPI_TX_END_REQ:
		return DEVAPI_TX_END_RESP;
	case DEVAPI_REBOOT_REQ:
		return DEVAPI_REBOOT_RESP;
	case DEVAPI_RESET_REQ:
		return DEVAPI_RESET_RESP;
	case DEVAPI_SW_IMAGE_DOWNLOAD_REQ:
		return DEVAPI_SW_IMAGE_DOWNLOAD_RESP;
	case DEVAPI_SW_UPDATE_REQ:
		return DEVAPI_SW_UPDATE_RESP;
	case DEVAPI_GET_SW_UPDATE_STATUS_REQ:
		return DEVAPI_GET_SW_UPDATE_STATUS_RESP;
	default:
		return DEVAPI_MSG_INVALID;
	}
}

static enum devapi_rsp_status
validate_req(const struct devapi_msg_hdr *hdr, const char *schema_id,
		const char **err_msg)
{
	// Check that we received complete hdr
	if ((hdr->msg_len < sizeof(struct devapi_msg_hdr)) ||
		(schema_id == NULL)) {
		LOG_ERROR("Incomplete message header received");
		*err_msg = "Incomplete message";
		return DEVAPI_STATUS_MSG_FORMAT_ERR;
	} else if (hdr->msg_len > AGT_MAX_MSGLEN) {
		LOG_ERROR("Message length [%d] > message limit [%d]",
			hdr->msg_len, AGT_MAX_MSGLEN);
		*err_msg = "Too big message";
		return DEVAPI_STATUS_MSG_FORMAT_ERR;
	} else if (!hdr->msg_last_fragment) {
		LOG_ERROR("Fragmented request is not supported");
		*err_msg = "Fragment message";
		return DEVAPI_STATUS_MSG_FORMAT_ERR;
	} else if ((hdr->msg_type != DEVAPI_HELLO_REQ) &&
			strcmp(schema_id, dm_schema_id)) {
		LOG_ERROR("Unsupported schema %s, supports %s",
				schema_id, dm_schema_id);
		*err_msg = "Unsupported schema_id";
		return DEVAPI_STATUS_SCHEMA_ERR;
	} else if (hdr->target_schema != DEVAPI_SCHEMA_CARDMGMT &&
			hdr->target_schema != DEVAPI_SCHEMA_CALIBRATION) {
		LOG_ERROR("Unsupported target schema %u",
			hdr->target_schema);
		*err_msg = "Unsupported schema";
		return DEVAPI_STATUS_SCHEMA_ERR;
	} else if (!is_valid_request_type(hdr->msg_type)) {
		LOG_ERROR("Unsupported request type: %d", hdr->msg_type);
		*err_msg = "Unsupported request";
		return DEVAPI_STATUS_REQ_UNSUPPORTED;
	}
	return DEVAPI_STATUS_SUCCESS;
}

static int
send_msg(const struct agt_buf *rsp, int *mgr_sock)
{
	//assert(mgr_sock != NULL);
	int ret = -1;
	const char *msg = rsp->buf;
	size_t msglen;
	
	msglen = be16toh(((const struct devapi_msg_hdr *)rsp->buf)->msg_len);

	while (1) {
		bool write_ready = false;
		int mgr_sock_dup = *mgr_sock;
		if (select_one(listen_sock, &mgr_sock_dup,
				false, &write_ready) != 0) {
			LOG_ERROR("select failed. Exiting the loop");
			return -1;
		}
		// Check if we got new conn
		if (mgr_sock_dup != *mgr_sock) {
			if (*mgr_sock == -1) {
				LOG_INFO("New conn %d", mgr_sock_dup);
			} else {
				LOG_INFO("Old conn [%d], Second conn [%d]",
					*mgr_sock, mgr_sock_dup);
				LOG_INFO("Closing conn %d", *mgr_sock);
				os_close(*mgr_sock);
			}
			*mgr_sock = mgr_sock_dup;
			return -1;
		} else if (write_ready) {
			ret = os_send(*mgr_sock, msg, msglen, 0);
			if (ret < 0) {
				if (ret == EAGAIN || ret == EWOULDBLOCK) {
					LOG_DEBUG("Incomplete send");
					continue;
				}
				LOG_ERROR("Failed to send msg, errno=%d", ret);
				LOG_ERROR("Closing socket %d", *mgr_sock);
				os_close(*mgr_sock);
				*mgr_sock = -1;
				return -1;
			}
			if (ret == msglen) {
				LOG_DEBUG("Sent msg");
				return 0;
			}
			msg += ret;
			msglen -= ret;
		}
	}
	return 0;
}

static int
make_err_rsp(enum devapi_rsp_status status, const char *app_err,
		const struct devapi_msg_hdr *hdr, struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;

	init_buf(rsp_buf);
	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, hdr->msg_id,
			get_resp_type(hdr->msg_type),
			hdr->target_schema, dm_schema_id);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack hdr in error response");
		init_buf(rsp_buf);
		return -1;
	}
	ret = devapi_msg_pack_status(&packer, status, app_err);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack status in error response");
		init_buf(rsp_buf);
		return -1;
	}

	dump_msg_hdr((struct devapi_msg_hdr *)rsp_buf->buf, dm_schema_id);
	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
process_hello_req(//const struct agt_msg *msg,
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;

	init_buf(rsp_buf);

	// No payload required in hello request
	if (req_hdr->msg_len > (sizeof(struct devapi_msg_hdr)
		+ strlen(schema_id) + 1)) {
		LOG_INFO("Ignoring the payload received");
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_HELLO_RESP, req_hdr->target_schema,
			dm_schema_id);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack header, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack status, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	// Set other payload fields
	const char *msg_version = AGT_MSG_VERSION;
	uint16_t msg_len_limit = AGT_MAX_MSGLEN;
	uint8_t card_size = get_card_size();
	if (((ret = devapi_msg_pack_ptv(&packer, "msg_version",
			DEVAPI_DATATYPE_STRING16, msg_version))
			== DEVAPI_SUCCESS) &&
		((ret = devapi_msg_pack_ptv(&packer, "msg_len_limit",
			DEVAPI_DATATYPE_UINT16, &msg_len_limit))
			== DEVAPI_SUCCESS) &&
		((ret = devapi_msg_pack_ptv(&packer, "card_size",
			DEVAPI_DATATYPE_UINT8, &card_size))
			== DEVAPI_SUCCESS)) {
	} else {
		LOG_ERROR("Failed to pack PTV, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
process_get_schema_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	struct tftp_server srv;

	init_buf(rsp_buf);

/*
	// No payload required in hello request
	if (req_hdr->msg_len <= sizeof(struct devapi_msg_hdr)) {
		LOG_INFO("No payload in the GET request");
	}
*/

	if (((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_srv_ip",
		DEVAPI_DATATYPE_STRING16, srv.ipaddr)) != DEVAPI_SUCCESS) ||
	    ((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_srv_port",
		DEVAPI_DATATYPE_UINT16, &srv.port)) != DEVAPI_SUCCESS) ||
	    ((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_path",
		DEVAPI_DATATYPE_STRING128, srv.path)) != DEVAPI_SUCCESS)) {
		LOG_ERROR("Failed to get TFTP params, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Failed to read payload", req_hdr, rsp_buf);
		return 0;
	}
	if ((ret = upload_schema_file(&srv)) != 0) {
		LOG_ERROR("Failed to upload schema files, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_APP_ERR,
				"Failed to upload schema files",
				req_hdr, rsp_buf);
		return 0;
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_GET_SCHEMA_RESP, req_hdr->target_schema,
			dm_schema_id);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack msghdr, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack status, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
process_get_param_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf,
		int *mgr_sock)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	bool packer_ready = false;
	uint16_t num_objs = 0;
	uint16_t num_params = 0;
	char obj_id[128];
	char param_id[16];
	int obj_iter = 0;
	int param_iter = 0;
	const struct schema_node *obj;

	init_buf(rsp_buf);
	//payload in request must
	if ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_objs",
		DEVAPI_DATATYPE_UINT16, &num_objs)) != DEVAPI_SUCCESS) {
		LOG_ERROR("Failed to pack payload, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Unpacking num_of_objs", req_hdr, rsp_buf);
		return 0;
	}
	LOG_VERBOSE("num_objs: %u", num_objs);
	for (obj_iter = 0; obj_iter < num_objs; obj_iter++) {
		// unpack obj_id & num_of_params
		if (((ret = devapi_msg_unpack_field(unpacker, "obj_id",
			DEVAPI_DATATYPE_STRING128, obj_id)) != DEVAPI_SUCCESS) ||
		    ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_params",
			DEVAPI_DATATYPE_UINT16, &num_params)) != DEVAPI_SUCCESS)) {
			LOG_ERROR("Failed to unpack obj_id/num_params,"
					" ret=%d", ret);
			make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
					"Failed to unpack obj_id/num_params",
					req_hdr, rsp_buf);
			return 0;
		}
		LOG_VERBOSE("obj_id[%d]: %s, num_params[%d]",
				obj_iter+1, obj_id, num_params);
		obj = dm_get_node(DM_TYPE_MGMT, obj_id);
		if (obj == NULL) {
			LOG_ERROR("Object %s not found in tree", obj_id);
			char err_msg[DEVAPI_ERR_MSG_LEN];
			snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s",
					"Bad object ", obj_id);
			make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
					req_hdr, rsp_buf);
			return 0;
		}
		//dm_print_param(obj);  // XXX: remove this line after debug

START_NEW_MSG_OBJ:
		if (!packer_ready) {
			devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
			packer_ready = true;
			ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
					DEVAPI_GET_PARAM_RESP, req_hdr->target_schema,
					dm_schema_id);
			if (ret != DEVAPI_SUCCESS) {
				LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
				make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
			ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
			if (ret != DEVAPI_SUCCESS) {
				LOG_ERROR("Failed to pack status, ret=%d", ret);
				make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
		}

		// pack obj_id & num_of_params
		if ((ret = devapi_msg_pack_obj(&packer, obj_id))
				!= DEVAPI_SUCCESS) {
			if (ret == DEVAPI_PACKER_NO_MEM) {
				// split the response
				if (obj_iter == 0) {
					LOG_ERROR("No space to pack even 1 object");
				}
				devapi_msg_mark_fragment(&packer);
				ret = send_msg(rsp_buf, mgr_sock);
				if (ret != 0) {
					LOG_ERROR("Failed to send msg fragment, ret=%d", ret);
					return -1;
				}
				init_buf(rsp_buf);
				packer_ready = false;
				goto START_NEW_MSG_OBJ;
			}
			LOG_ERROR("Internal error packing %s, ret=%d",
					obj_id, ret);
			make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
					req_hdr, rsp_buf);
			return 0;
		}
		for (param_iter = 0; param_iter < num_params; param_iter++) {
			// Unpack param
			if ((ret = devapi_msg_unpack_field(unpacker, "param_id",
				DEVAPI_DATATYPE_STRING16, param_id)) != DEVAPI_SUCCESS) {
				LOG_ERROR("Failed to unpack param_id,"
						" ret=%d", ret);
				make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
						"Failed to unpack param_id",
						req_hdr, rsp_buf);
				return 0;
			}
			LOG_VERBOSE("param_id[%d][%d]: %s",
					obj_iter+1, param_iter+1, param_id);
			const struct schema_node *param = get_child_schema_node(obj, param_id, strlen(param_id));
			if (param == NULL) {
				LOG_ERROR("Param %s/%s not found in tree",
						obj_id, param_id);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s",
						"Bad param ", obj_id, param_id);
				make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				return 0;
			}
			//dm_print_param(param);  // XXX: remove this line after debug

START_NEW_MSG_PARAM:
			if (!packer_ready) {
				devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
				packer_ready = true;
				ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
						DEVAPI_GET_PARAM_RESP, req_hdr->target_schema,
						dm_schema_id);
				if (ret != DEVAPI_SUCCESS) {
					LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
					make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
							req_hdr, rsp_buf);
					return 0;
				}
				ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
				if (ret != DEVAPI_SUCCESS) {
					LOG_ERROR("Failed to pack status, ret=%d", ret);
					make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
							req_hdr, rsp_buf);
					return 0;
				}
				// Pack obj header
				if ((ret = devapi_msg_pack_obj(&packer, obj_id))
						!= DEVAPI_SUCCESS) {
					LOG_ERROR("Internal error packing %s, ret=%d",
							obj_id, ret);
					make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
							req_hdr, rsp_buf);
					return 0;
				}
			}

			// pack param
			if ((ret = devapi_msg_pack_obj_param(&packer, param_id,
					param->datatype, param->val))
					!= DEVAPI_SUCCESS) {
				if (ret == DEVAPI_PACKER_NO_MEM) {
					// split the response
					if (obj_iter == 0 && param_iter == 0) {
						LOG_ERROR("No space to pack even 1st param");
					}
					devapi_msg_mark_fragment(&packer);
					ret = send_msg(rsp_buf, mgr_sock);
					if (ret != 0) {
						LOG_ERROR("Failed to send msg fragment, ret=%d", ret);
						return -1;
					}
					init_buf(rsp_buf);
					packer_ready = false;
					goto START_NEW_MSG_PARAM;
				}
				LOG_ERROR("Internal error packing %s,"
						" ret=%d", obj_id, ret);
				make_err_rsp(DEVAPI_STATUS_APP_ERR,
						"Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
		}
	}

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
process_req(const struct agt_buf *req_buf, int *mgr_sock,
		struct agt_buf *rsp_buf)
{
	struct devapi_msg_hdr req_hdr;
	const char *schema_id = NULL;
	struct devapi_msg_packer_ctx unpacker;
	enum devapi_rsp_status rsp_status;
	enum devapi_return ret;
	const struct agt_msg *msg = (struct agt_msg *)req_buf->buf;
	const char *err_msg = NULL;
	// int ret = -1;

	// req->buf is const, packer needs non-const
	devapi_msg_packer_init(&unpacker, (void*)req_buf->buf, req_buf->filled);
	memset(&req_hdr, 0, sizeof(req_hdr));
	ret = devapi_msg_unpack_msghdr(&unpacker, &req_hdr, &schema_id);
	if (ret != DEVAPI_SUCCESS) {
		LOG_ERROR("Unpack msghdr failed, ret=%d", ret);
		make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Failed to read msghdr", &req_hdr, rsp_buf);
		return 0;
	}
	dump_msg_hdr(&req_hdr, schema_id);

	init_buf(rsp_buf);

	rsp_status = validate_req(&req_hdr, schema_id, &err_msg);
	if (rsp_status != DEVAPI_STATUS_SUCCESS) {
		make_err_rsp(rsp_status, err_msg, &req_hdr, rsp_buf);
		return 0;
	}

	switch (msg->hdr.msg_type) {
	case DEVAPI_HELLO_REQ:
		ret = process_hello_req(&unpacker, &req_hdr, schema_id,
				rsp_buf);
		break;
	case DEVAPI_GET_SCHEMA_REQ:
		ret = process_get_schema_req(&unpacker, &req_hdr, schema_id,
				rsp_buf);
		break;
	case DEVAPI_GET_PARAM_REQ:
		ret = process_get_param_req(&unpacker, &req_hdr, schema_id,
				rsp_buf, mgr_sock);
		break;
	case DEVAPI_GET_UPDATED_PARAM_REQ:
	case DEVAPI_TX_START_REQ:
	case DEVAPI_SET_CONFIG_REQ:
	case DEVAPI_TX_END_REQ:
	case DEVAPI_REBOOT_REQ:
	case DEVAPI_RESET_REQ:
	case DEVAPI_SW_IMAGE_DOWNLOAD_REQ:
	case DEVAPI_SW_UPDATE_REQ:
	case DEVAPI_GET_SW_UPDATE_STATUS_REQ:
		break;
	default:
		ret = -1;
		break;
	}

	if (ret == 0) {
	}

	return ret;
}

/*
 * Return values
 *  -1 = error
 *   0 = read one partail / full message
 */
static int
read_msg(int fd, struct agt_buf *r_buf)
{
	static int nbytes = 0;
	size_t pending;
	bool big_msg = false;

	const struct devapi_msg_hdr *hdr = (struct devapi_msg_hdr *)r_buf->buf;
	if (r_buf->filled == 0) {
		//assert(hdr->msg_len == 0);
		pending = sizeof(hdr->msg_len);
	} else if (r_buf->filled < sizeof(hdr->msg_len)) {
		// msg_len is not read; first read msg_len
		pending = sizeof(hdr->msg_len) - r_buf->filled;
	} else {
		if (r_buf->filled < be16toh(hdr->msg_len)) {
			// Previously paritail message retrieved
			pending = be16toh(hdr->msg_len) - r_buf->filled;
			if (be16toh(hdr->msg_len) > AGT_MAX_MSGLEN) {
				// Oh, too big message
				big_msg = true;
				if (pending > (AGT_MAX_MSGLEN - sizeof(*hdr))) {
					// read as much as buffer allows
					pending = AGT_MAX_MSGLEN - sizeof(*hdr);
				}
				// NOTE: might have not read hdr/schema_id fully
			}
		} else {
			LOG_ERROR("Caller did not reset buffer");
			return -1;
		}
	}

	LOG_INFO("pending=%d", pending);
	if (!big_msg) {
		nbytes = os_recv(fd, (r_buf->buf + r_buf->filled), pending, 0);
	} else {
		// Let us drain the socket, let us keep overwriting
		// the schema_id + payload portion
		nbytes = os_recv(fd, (r_buf->buf + sizeof(*hdr)), pending, 0);
	}
	if (nbytes < 0) {
		LOG_ERROR("Read(fd=%d) failed, errno=%d", fd, errno);
		return -1;
	} else if (nbytes == 0) {
		LOG_INFO("End-of-file for %d", fd);
		return -1;
	}
	r_buf->filled += nbytes;
	if (r_buf->filled < sizeof(hdr->msg_len)) {
		LOG_VERBOSE("Read %d bytes, fd %d, total %d",
				nbytes, fd, r_buf->filled);
		return 0;
	}
	if (r_buf->filled < be16toh(hdr->msg_len)) {
		LOG_VERBOSE("Read %d bytes, fd %d, total %d, pending %d",
				nbytes, fd, r_buf->filled,
				(be16toh(hdr->msg_len) - r_buf->filled));
		return 0; /* continue reading */
	} else {
		LOG_VERBOSE("Read %d bytes from fd %d, msg_len %d",
				nbytes, fd, be16toh(hdr->msg_len));
		LOG_INFO("Recevied msg %d of length %d from fd %d",
				hdr->msg_type,
				be16toh(hdr->msg_len), fd);
	}
	return 0;
}

static void
run_agent(int listen_sock, struct agt_buf *req_buf, struct agt_buf *rsp_buf)
{
	int mgr_sock = -1;
	int ret = -1;
	bool read_ready;
	int mgr_sock_dup;

	init_buf(req_buf);
	init_buf(rsp_buf);

	while (1) {
		read_ready = false;
		mgr_sock_dup = mgr_sock;
		if (select_one(listen_sock, &mgr_sock_dup,
				true, &read_ready) != 0) {
			LOG_ERROR("select failed. Exiting the loop");
			return;
		}
		if (mgr_sock_dup != mgr_sock) {
			if (mgr_sock == -1) {
				LOG_INFO("New conn %d", mgr_sock_dup);
			} else {
				LOG_INFO("Old conn [%d], Second conn [%d]",
					mgr_sock, mgr_sock_dup);
				LOG_INFO("Closing conn %d", mgr_sock);
				os_close(mgr_sock);
			}
			mgr_sock = mgr_sock_dup;
			init_buf(req_buf);
		} else if (read_ready) {
			// Read message
			if (read_msg(mgr_sock, req_buf) == -1) {
				LOG_INFO("Closing conn %d", mgr_sock);
				os_close(mgr_sock);
				mgr_sock = -1;
			}
			if (!is_req_msg_complete(req_buf)) {
				continue;
			}
			// Process request
			init_buf(rsp_buf);
			ret = process_req(req_buf, &mgr_sock, rsp_buf);
			init_buf(req_buf);
			if (ret != 0) {
				LOG_ERROR("process_req failed, ret=%d", ret);
				continue;
			}
			// Send response
			ret = send_msg(rsp_buf, &mgr_sock);
			if (ret != 0) {
				LOG_ERROR("send_msg failed, ret=%d", ret);
				continue;
			}
		}
		
	}
}

int dev_agent_process(void)
{
	uint16_t listen_port = AGT_SERVER_PORT;
	
	LOG_INFO("Listening on port %u", listen_port);
	listen_sock = os_init_listen_sock(INADDR_ANY, listen_port);

	if (listen_sock == -1) {
		LOG_ERROR("Failed to init listen sock");
		return -1;
	}

	LOG_DEBUG("Entering infinite loop");
	run_agent(listen_sock, &g_req_buf, &g_rsp_buf);

	return 0;
}
