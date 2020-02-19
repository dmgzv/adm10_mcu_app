#ifndef _DEVAPI_PACKER_H_
#define _DEVAPI_PACKER_H_

#include "devapi.h"

#define S_NUM_OF_OBJS	"num_of_objs"
#define S_NUM_OF_PARAMS	"num_of_params"

struct devapi_msg_packer_ctx {
	uint8_t hdr_done;
	char *hdr_ptr;
	char *curr_buf_ptr;
	char *obj_start_ptr;
	char *param_start_ptr;
	size_t curr_buf_size;
	size_t max_buf_size;
};

/* devapi_msg_packer_init
 * 		Initializes DevAPI packer -- called for each message parsing.
 * 		Mandatory to call this before pack and unpack APIs
 * Args:
 * 		[IN] buf: Buffer provided by caller where the message is filled.
 * 			  Memory alloc done by calling app.  For packing, new
 * 			  char ptr/array for storing the buf is passed.  While
 * 			  unpacking, the received buffer is passed.
 *		[IN] buf_size: Size of the buffer provided
 *		[OUT] ctx: Message packer context
 */
void devapi_msg_packer_init(struct devapi_msg_packer_ctx *ctx,
		void *buf, size_t buf_size);

/* devapi_msg_pack_msghdr
 * 		Set the header info in the buffer
 * 		Sets following default values in the header:
 * 			msg_len - Calculated internally and set
 * 			target_schema = 0 (Schema for card mgmt)
 * 			id_notation = 0 (default notation XPATH)
 * 			msg_last_fragment = 1 (default "is a last msg")
 * Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN] msg_id: Uniqie message id set by caller to id the req/resp.
 * 		[IN] msg_type: Message type
 * 		[IN] schema_id: Schema id
 */
enum devapi_return devapi_msg_pack_msghdr(struct devapi_msg_packer_ctx *ctx,
		uint32_t msg_id,
		enum devapi_msg_type msg_type,
		enum devapi_schema_type target_schema,
		const char *schema_id);


/* devapi_msg_pack_status
 * 		Set the status info in the buffer
 * 		Packs err_msg only if status != DEVAPI_STATUS_SUCCESS
 * Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN] status: Response status
 * 		[IN] err_msg: error message in the response.
 */
enum devapi_return devapi_msg_pack_status(struct devapi_msg_packer_ctx *ctx,
		enum devapi_rsp_status status,
		const char *err_msg);

/* devapi_msg_mark_fragment
 * 		Marks that msg is a fragment
 * 		Note: After packing hdr user has to call
 * 		devapi_msg_mark_fragment the packed msg as fragment.
 * Args:
 * 		[IN-OUT] ctx: Parser context
 */
void devapi_msg_mark_fragment(struct devapi_msg_packer_ctx *ctx);


/* devapi_msg_pack_ptv
 * 		Sets the name, type-value in the payload
 * Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN] name: Field name
 * 		[IN] datatype: Datatype of the value
 * 		[IN] value: Value for the given name
 */
enum devapi_return devapi_msg_pack_ptv(struct devapi_msg_packer_ctx *ctx,
		const char *name,
		enum devapi_datatype datatype,
		const void *val);

/* devapi_msg_pack_string
 * 		Packs given string into msg
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN] str: str to be packed
 */
enum devapi_return devapi_msg_pack_string(struct devapi_msg_packer_ctx *ctx,
		const char *str);

/* devapi_msg_pack_obj
 * 		Sets the object with necessary object and param header with
 * 		zero nparams For object, full XPATH is provided.
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN] xpath: Xpath of the object or parameter
 */
enum devapi_return devapi_msg_pack_obj(struct devapi_msg_packer_ctx *ctx,
		const char *xpath);

/* devapi_msg_pack_obj_param
 * 		Packs the obj param
 *		If val is not NULL, packs datatype and val
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN] param_name
 *		[IN] param_type
 *		[IN] val
 */
enum devapi_return devapi_msg_pack_obj_param(struct devapi_msg_packer_ctx *ctx,
		const char *param_name,
		enum devapi_datatype param_type,
		const void *val);

/* devapi_msg_unpack_msghdr
 * 		Unpack the message header from the received buffer.
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[OUT] hdr: Filled devapi message header structure
 * 		[OUT] schema_id: schema_id
 */
enum devapi_return devapi_msg_unpack_msghdr(struct devapi_msg_packer_ctx *ctx,
		struct devapi_msg_hdr *hdr, const char **schema_id);

/* devapi_msg_unpack_status
 * 		Unpack the status from the received response buffer.
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[OUT] result: Filled devapi status structure
 */
enum devapi_return devapi_msg_unpack_status(struct devapi_msg_packer_ctx *ctx,
		struct devapi_req_result *result);


/* devapi_msg_unpack_ptv
 * 		Extracts the name, type-value from the buffer
 * Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[OUT] name: Field name
 * 		[OUT] name_len : name_len
 * 		[OUT] datatype: Datatype of the value
 * 		[OUT] val: Value for the given name
 * 		[OUT] val_len: value len
 */
enum devapi_return devapi_msg_unpack_ptv(struct devapi_msg_packer_ctx *ctx,
		char *name, size_t name_len,
		enum devapi_datatype *datatype,
		void *val, size_t val_len);

enum devapi_return devapi_msg_unpack_ptv_val(struct devapi_msg_packer_ctx *ctx,
		const char *name, enum devapi_datatype datatype, void *val);

/* devapi_msg_unpack_value
 * 		Extracts specific field value from the buffer
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN]	 datatype: Datatype of the value
 * 		[OUT]	value: Value to be extracted
 */
/* devapi_msg_unpack_value
 * 		Extracts specific field value from the buffer
 * 	Args:
 * 		[IN-OUT] ctx: Parser context
 * 		[IN]	 datatype: Datatype of the value
 * 		[OUT]	value: Value to be extracted
 */
enum devapi_return devapi_msg_unpack_field(struct devapi_msg_packer_ctx *ctx,
		const char *field,
		enum devapi_datatype datatype,
		void *val);

// Util functions:
int devapi_get_val_len(enum devapi_datatype datatype, const void *val);

#endif

