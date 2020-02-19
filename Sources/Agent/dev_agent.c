#include "devapi.h"
#include "devapi_packer.h"
#include "dev_agent_conf.h"
#include "pl.h"
#include "dm.h"
#include "agt.h"

extern struct agt g_agt;
extern struct dm g_dm;

/*
 * devapi_msg_hdr is used when only hdr is populated.
 * agt_msg is used when it has payload also populated.
 */
struct agt_msg {
	struct devapi_msg_hdr hdr;
};


static void
agt_init_buf(struct agt_buf *r_buf)
{
	memset(r_buf, 0, sizeof(*r_buf));
}

static void
agt_dump_msg_hdr(const struct devapi_msg_hdr *hdr, const char *schema_id)
{
	PL_LOG_VERBOSE("msg_type=%d, msg_id=%d, msg_len=%d, target_schema=%d,"
			" id_notation=%d, fragment=%d, schema_id=%s",
			hdr->msg_type, be32toh(hdr->msg_id),
			be16toh(hdr->msg_len),
			hdr->target_schema, hdr->id_notation,
			hdr->msg_last_fragment,
			(schema_id) ? schema_id : "");
}

static bool
agt_is_req_msg_complete(const struct agt_buf *buf)
{
	const struct devapi_msg_hdr *hdr = (struct devapi_msg_hdr *)buf->buf;
	return ((buf->filled >= sizeof(hdr->msg_len)) &&
			(buf->filled == be16toh(hdr->msg_len)));
}

static bool
agt_is_valid_request_type(enum devapi_msg_type type)
{
	switch (type) {
	case DEVAPI_HELLO_REQ:
	case DEVAPI_GET_SCHEMA_REQ:
	case DEVAPI_GET_PARAM_REQ:
	case DEVAPI_GET_UPDATED_PARAM_REQ:
	case DEVAPI_TX_START_REQ:
	case DEVAPI_SET_CONFIG_REQ:
	case DEVAPI_TX_END_REQ:
	case DEVAPI_REBOOT_REQ:
	case DEVAPI_RESET_REQ:
	case DEVAPI_SW_IMAGE_DOWNLOAD_REQ:
	case DEVAPI_SW_UPDATE_REQ:
	case DEVAPI_GET_SW_UPDATE_STATUS_REQ:
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
agt_get_resp_type(enum devapi_msg_type req_type)
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
agt_validate_req(const struct devapi_msg_hdr *hdr, const char *schema_id,
		const char **err_msg)
{
	// Check that we received complete hdr
	if ((hdr->msg_len < sizeof(struct devapi_msg_hdr)) ||
		(schema_id == NULL)) {
		PL_LOG_ERROR("Incomplete message header received");
		*err_msg = "Incomplete message";
		return DEVAPI_STATUS_MSG_FORMAT_ERR;
	} else if (hdr->msg_len > AGT_MAX_MSGLEN) {
		PL_LOG_ERROR("Message length [%d] > message limit [%d]",
			hdr->msg_len, AGT_MAX_MSGLEN);
		*err_msg = "Too big message";
		return DEVAPI_STATUS_MSG_FORMAT_ERR;
	} else if (!hdr->msg_last_fragment) {
		PL_LOG_ERROR("Fragmented request is not supported");
		*err_msg = "Fragment message";
		return DEVAPI_STATUS_MSG_FORMAT_ERR;
	} else if ((hdr->msg_type != DEVAPI_HELLO_REQ) &&
			strcmp(schema_id, g_dm.schema_id)) {
		PL_LOG_ERROR("Unsupported schema %s, supports %s",
				schema_id, g_dm.schema_id);
		*err_msg = "Unsupported schema_id";
		return DEVAPI_STATUS_SCHEMA_ERR;
	} else if (hdr->target_schema != DEVAPI_SCHEMA_CARDMGMT &&
			hdr->target_schema != DEVAPI_SCHEMA_CALIBRATION) {
		PL_LOG_ERROR("Unsupported target schema %u",
			hdr->target_schema);
		*err_msg = "Unsupported schema";
		return DEVAPI_STATUS_SCHEMA_ERR;
	} else if (!agt_is_valid_request_type(hdr->msg_type)) {
		PL_LOG_ERROR("Unsupported request type: %d", hdr->msg_type);
		*err_msg = "Unsupported request";
		return DEVAPI_STATUS_REQ_UNSUPPORTED;
	}

	// Check request specific validation
	/*
	if (hdr->msg_type == DEVAPI_TX_END_REQ) {
		if (hdr->target_schema == DEVAPI_SCHEMA_CALIBRATION) {
			// commit/rollback is not supported for CALIBRATION schema
			PL_LOG_ERROR("Target schema %u not supported in TX_END",
				hdr->target_schema);
			*err_msg = "Calibration schema is not supported in TX_END";
			return DEVAPI_STATUS_SCHEMA_ERR;
		}
	}
	*/
	return DEVAPI_STATUS_SUCCESS;
}


//XXX: check target_schema is not used, both MGMT and CALIBRATION are rolledback
static int
agt_rollback(uint8_t target_schema)
{
	g_agt.in_trans = false;
	if (dm_load_startup_config(g_dm.schema_id, target_schema,
				g_dm.mgmt->config.values,
				g_dm.mgmt->config.values_size,
				g_dm.calib->config.values,
				g_dm.calib->config.values_size) != 0) {
		PL_LOG_ERROR("Failed to load config. Will reset to defaults.");
		if (dm_reset_config_tree(&g_dm,
				dm_get_root_node(&g_dm, DM_TYPE_MGMT)) != 0) {
			return -1;
		}
		if (dm_reset_config_tree(&g_dm,
				dm_get_root_node(&g_dm, DM_TYPE_CALIBRATION)) != 0) {
			return -1;
		}
	}
	PL_LOG_INFO("Transaction is rolled back");
	return 0;
}

//XXX: check target_schema is not used, both MGMT and CALIBRATION are committed
static int
agt_commit(uint8_t target_schema)
{
	g_agt.in_trans = false;
	PL_LOG_INFO("Committing transaction");
	if (dm_write_startup_config(g_dm.schema_id, target_schema,
				g_dm.mgmt->config.values,
				g_dm.mgmt->config.values_size,
				g_dm.calib->config.values,
				g_dm.calib->config.values_size) != 0) {
		PL_LOG_ERROR("Failed to save config.");
		if (agt_rollback(target_schema) != 0) {
			PL_LOG_ERROR("Commit failed. Rollback failed. Exiting.");
			//exit(0);
		}
		PL_LOG_INFO("Commit failed, and transaction is rolled back");
	} else {
		PL_LOG_INFO("Commit successful");
	}
	return 0;
}

static int
do_reboot()
{
	// TODO: implement real reboot
	// rollback any in-progress transaction (until we implement actual reboot)
	if (g_agt.in_trans) {
		agt_rollback(DM_TYPE_MGMT);
	}

	g_agt.sw_update_state = DEVAPI_SU_STATUS_IDLE;

	//sleep(3);  // let any outgoing messages complete

	if (g_agt.mgr_sock != -1) {
		PL_LOG_INFO("Closing conn %d", g_agt.mgr_sock);
		pl_close(g_agt.mgr_sock);
		g_agt.mgr_sock = -1;
	}

	pl_restart_process(g_agt.listen_port);
}

static int
agt_send_msg(const struct agt_buf *rsp, int *mgr_sock)
{
	pl_assert(mgr_sock != NULL);
	int ret = -1;
	const char *msg = rsp->buf;
	size_t msglen;

	if (*mgr_sock == -1) {
		PL_LOG_INFO("No connection to manager");
		return -1;
	}
	
	msglen = be16toh(((const struct devapi_msg_hdr *)rsp->buf)->msg_len);

	while (1) {
		bool write_ready = false;
		int mgr_sock_dup = *mgr_sock;
		if (pl_select_one(g_agt.agt_listen_sock, &mgr_sock_dup,
				false, &write_ready) != 0) {
			PL_LOG_ERROR("select failed. Exiting the loop");
			return -1;
		}
		// Check if we got new conn
		if (mgr_sock_dup != *mgr_sock) {
			if (*mgr_sock == -1) {
				PL_LOG_INFO("New conn %d", mgr_sock_dup);
			} else {
				PL_LOG_INFO("Old conn [%d], Second conn [%d]",
					*mgr_sock, mgr_sock_dup);
				PL_LOG_INFO("Closing conn %d", *mgr_sock);
				pl_close(*mgr_sock);
				*mgr_sock = -1;
				// Close transaction and rollback
				if (g_agt.in_trans == true) {
					agt_rollback(DM_TYPE_MGMT);
				}
			}
			*mgr_sock = mgr_sock_dup;
			return -1;
		} else if (write_ready) {
			ret = pl_send(*mgr_sock, msg, msglen, 0);
			if (ret < 0) {
				if (ret == PL_EAGAIN || ret == PL_EWOULDBLOCK) {
					PL_LOG_DEBUG("Incomplete send");
					continue;
				}
				PL_LOG_ERROR("Failed to send msg, pl_errno=%d", ret);
				PL_LOG_ERROR("Closing socket %d", *mgr_sock);
				pl_close(*mgr_sock);
				*mgr_sock = -1;
				// Close transaction and rollback
				if (g_agt.in_trans == true) {
					agt_rollback(DM_TYPE_MGMT);
				}
				return -1;
			}
			if (ret == msglen) {
				PL_LOG_DEBUG("Sent msg - msglen=%d", msglen);
				return 0;
			}
			msg += ret;
			msglen -= ret;
		}
	}
	return 0;
}

static int
agt_make_err_rsp(enum devapi_rsp_status status, const char *app_err,
		const struct devapi_msg_hdr *hdr, struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;

	agt_init_buf(rsp_buf);
	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, hdr->msg_id,
			agt_get_resp_type(hdr->msg_type),
			hdr->target_schema, g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack hdr in error response");
		agt_init_buf(rsp_buf);
		return -1;
	}
	ret = devapi_msg_pack_status(&packer, status, app_err);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status in error response");
		agt_init_buf(rsp_buf);
		return -1;
	}

	agt_dump_msg_hdr((struct devapi_msg_hdr *)rsp_buf->buf, g_dm.schema_id);
	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_hello_req(//const struct agt_msg *msg,
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;

	agt_init_buf(rsp_buf);

	// No payload required in hello request
	if (req_hdr->msg_len > (sizeof(struct devapi_msg_hdr)
		+ strlen(schema_id) + 1)) {
		PL_LOG_INFO("Ignoring the payload received");
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_HELLO_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack header, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	// Set other payload fields
	const char *msg_version = AGT_MSG_VERSION;
	uint16_t msg_len_limit = AGT_MAX_MSGLEN;
	uint8_t card_size = g_dm.card_size;
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
		PL_LOG_ERROR("Failed to pack PTV, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
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
agt_process_get_schema_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	struct pl_tftp_server srv;

	agt_init_buf(rsp_buf);

/*
	// No payload required in hello request
	if (req_hdr->msg_len <= sizeof(struct devapi_msg_hdr)) {
		PL_LOG_INFO("No payload in the GET request");
	}
*/

	if (((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_srv_ip",
		DEVAPI_DATATYPE_STRING16, srv.ipaddr)) != DEVAPI_SUCCESS) ||
	    ((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_srv_port",
		DEVAPI_DATATYPE_UINT16, &srv.port)) != DEVAPI_SUCCESS) ||
	    ((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_path",
		DEVAPI_DATATYPE_STRING128, srv.path)) != DEVAPI_SUCCESS)) {
		PL_LOG_ERROR("Failed to get TFTP params, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Failed to read payload", req_hdr, rsp_buf);
		return 0;
	}
	if ((ret = pl_upload_schema_file(&srv)) != 0) {
		PL_LOG_ERROR("Failed to upload schema files, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
				"Failed to upload schema files",
				req_hdr, rsp_buf);
		return 0;
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_GET_SCHEMA_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack msghdr, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
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
agt_process_get_param_req(
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

	agt_init_buf(rsp_buf);
	//payload in request must
	if ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_objs",
		DEVAPI_DATATYPE_UINT16, &num_objs)) != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to unpack payload, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Unpacking num_of_objs", req_hdr, rsp_buf);
		return 0;
	}
	PL_LOG_VERBOSE("num_objs: %u", num_objs);
	for (obj_iter = 0; obj_iter < num_objs; obj_iter++) {
		// unpack obj_id & num_of_params
		if (((ret = devapi_msg_unpack_field(unpacker, "obj_id",
			DEVAPI_DATATYPE_STRING128, obj_id)) != DEVAPI_SUCCESS) ||
		    ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_params",
			DEVAPI_DATATYPE_UINT16, &num_params)) != DEVAPI_SUCCESS)) {
			PL_LOG_ERROR("Failed to unpack obj_id/num_params,"
					" ret=%d", ret);
			agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
					"Failed to unpack obj_id/num_params",
					req_hdr, rsp_buf);
			return 0;
		}
		PL_LOG_VERBOSE("obj_id[%d]: %s, num_params[%d]",
				obj_iter+1, obj_id, num_params);
		const struct schema_node *obj = dm_get_node(&g_dm, req_hdr->target_schema, obj_id);
		if (obj == NULL) {
			PL_LOG_ERROR("Object %s not found in tree", obj_id);
			char err_msg[DEVAPI_ERR_MSG_LEN];
			pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s",
					"Bad object ", obj_id);
			agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
					req_hdr, rsp_buf);
			return 0;
		}
		//dm_print_param(&g_dm, obj, pl_printf, stdout);  // XXX: remove this line after debug

START_NEW_MSG_OBJ:
		if (!packer_ready) {
			devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
			packer_ready = true;
			ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
					DEVAPI_GET_PARAM_RESP, req_hdr->target_schema,
					g_dm.schema_id);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
			ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
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
					PL_LOG_ERROR("No space to pack even 1 object");
				}
				devapi_msg_mark_fragment(&packer);
				ret = agt_send_msg(rsp_buf, mgr_sock);
				if (ret != 0) {
					PL_LOG_ERROR("Failed to send msg fragment, ret=%d", ret);
					return -1;
				}
				agt_init_buf(rsp_buf);
				packer_ready = false;
				goto START_NEW_MSG_OBJ;
			}
			PL_LOG_ERROR("Internal error packing %s, ret=%d",
					obj_id, ret);
			agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
					req_hdr, rsp_buf);
			return 0;
		}
		for (param_iter = 0; param_iter < num_params; param_iter++) {
			// Unpack param
			if ((ret = devapi_msg_unpack_field(unpacker, "param_id",
				DEVAPI_DATATYPE_STRING16, param_id)) != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to unpack param_id,"
						" ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
						"Failed to unpack param_id",
						req_hdr, rsp_buf);
				return 0;
			}
			PL_LOG_VERBOSE("param_id[%d][%d]: %s",
					obj_iter+1, param_iter+1, param_id);
			const struct schema_node *param = dm_get_child_schema_node(obj, param_id, strlen(param_id));
			if (param == NULL) {
				PL_LOG_ERROR("Param %s/%s not found in tree",
						obj_id, param_id);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s",
						"Bad param ", obj_id, param_id);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				return 0;
			}
			//dm_print_param(&g_dm, param, pl_printf, stdout);  // XXX: remove this line after debug

START_NEW_MSG_PARAM:
			if (!packer_ready) {
				devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
				packer_ready = true;
				ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
						DEVAPI_GET_PARAM_RESP, req_hdr->target_schema,
						g_dm.schema_id);
				if (ret != DEVAPI_SUCCESS) {
					PL_LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
					agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
							req_hdr, rsp_buf);
					return 0;
				}
				ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
				if (ret != DEVAPI_SUCCESS) {
					PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
					agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
							req_hdr, rsp_buf);
					return 0;
				}
				// Pack obj header
				if ((ret = devapi_msg_pack_obj(&packer, obj_id))
						!= DEVAPI_SUCCESS) {
					PL_LOG_ERROR("Internal error packing %s, ret=%d",
							obj_id, ret);
					agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
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
						PL_LOG_ERROR("No space to pack even 1st param");
					}
					devapi_msg_mark_fragment(&packer);
					ret = agt_send_msg(rsp_buf, mgr_sock);
					if (ret != 0) {
						PL_LOG_ERROR("Failed to send msg fragment, ret=%d", ret);
						return -1;
					}
					agt_init_buf(rsp_buf);
					packer_ready = false;
					goto START_NEW_MSG_PARAM;
				}
				PL_LOG_ERROR("Internal error packing %s,"
						" ret=%d", obj_id, ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
						"Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
			// param is read; clear updated flag
			if (param->val_updated) {
				PL_LOG_DEBUG("Clearing val_updated flag %s/%s", obj_id, param_id);
				*param->val_updated = 0;
			}
		}
	}

	return 0;
}


/*
 * returns 0 if successful (even if input request processing failed)
 * returns 1 if error message prepared
 * return -1 only in case of abnormal situation.
 */
static int
agt_get_updated_param(
		const struct dm_schema_node *node,
		char *parent_xpath,
		uint8_t param_category,
		struct devapi_msg_packer_ctx *packer,
		bool *packer_ready,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf,
		int *mgr_sock)
{
	enum devapi_return ret;
	
	if (node->category != SCHEMA_NODE_TYPE_ROOT &&
			node->category != SCHEMA_NODE_TYPE_OBJECT) {
		return 0;
	}

	// First Step: Process parameters
	int packed_param_cnt = 0;
	bool obj_packed = false;
	for (int i = 0; i < node->num_children; i++) {
		const struct dm_schema_node *child = ((const struct dm_schema_node **)node->val)[i];
		if ((child->category != param_category) ||
				(child->val_updated == NULL) ||
				(*child->val_updated != 1)) {
			continue;
		}
		PL_LOG_VERBOSE("Found updated-param %s/%s", parent_xpath, child->label);

START_NEW_MSG_OBJ:
		if (!*packer_ready) {
			devapi_msg_packer_init(packer, rsp_buf->buf, sizeof(rsp_buf->buf));
			*packer_ready = true;
			ret = devapi_msg_pack_msghdr(packer, req_hdr->msg_id,
					DEVAPI_GET_UPDATED_PARAM_RESP, req_hdr->target_schema,
					g_dm.schema_id);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 1;
			}
			ret = devapi_msg_pack_status(packer, DEVAPI_STATUS_SUCCESS, NULL);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 1;
			}
			obj_packed = false;
			packed_param_cnt = 0;
		}

		// NOTE: ROOT_NODE does not have parameters, so it is OK to not touch parent_xpath
		// pack obj_id & num_of_params
		if (obj_packed == false &&
				(ret = devapi_msg_pack_obj(packer, parent_xpath))
				!= DEVAPI_SUCCESS) {
			if (ret == DEVAPI_PACKER_NO_MEM) {
				// split the response
				if (packed_param_cnt == 0) {
					PL_LOG_ERROR("No space to pack even 1 object");
				}
				devapi_msg_mark_fragment(packer);
				ret = agt_send_msg(rsp_buf, mgr_sock);
				if (ret != 0) {
					PL_LOG_ERROR("Failed to send msg fragment, ret=%d", ret);
					return -1;
				}
				agt_init_buf(rsp_buf);
				*packer_ready = false;
				goto START_NEW_MSG_OBJ;
			}
			PL_LOG_ERROR("Internal error packing %s, ret=%d",
					parent_xpath, ret);
			agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
					req_hdr, rsp_buf);
			return 1;
		}
		obj_packed = true;

		// pack param
		if ((ret = devapi_msg_pack_obj_param(packer, child->label,
				child->datatype, child->val))
				!= DEVAPI_SUCCESS) {
			if (ret == DEVAPI_PACKER_NO_MEM) {
				// split the response
				if (packed_param_cnt == 0) {
					PL_LOG_ERROR("No space to pack even 1st param");
				}
				devapi_msg_mark_fragment(packer);
				ret = agt_send_msg(rsp_buf, mgr_sock);
				if (ret != 0) {
					PL_LOG_ERROR("Failed to send msg fragment, ret=%d", ret);
					return -1;
				}
				agt_init_buf(rsp_buf);
				*packer_ready = false;
				goto START_NEW_MSG_OBJ;
			}
			PL_LOG_ERROR("Internal error packing %s/%s, ret=%d",
					parent_xpath, child->label, ret);
			agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
					"Internal error", req_hdr, rsp_buf);
			return 1;
		}
		packed_param_cnt++;

		// param is read; clear updated flag
		if (child->val_updated) {
			PL_LOG_DEBUG("Clearing val_updated flag %s/%s",
					parent_xpath, child->label);
			*child->val_updated = 0;
		}
	}

	// Second Step: Process objects
	for (int i = 0; i < node->num_children; i++) {
		const struct dm_schema_node *child = ((const struct dm_schema_node **)node->val)[i];
		if (child->category != SCHEMA_NODE_TYPE_ROOT &&
				child->category != SCHEMA_NODE_TYPE_OBJECT) {
			continue;
		}
		strncat(parent_xpath, "/", DEVAPI_XPATH_LEN);
		strncat(parent_xpath, child->label, DEVAPI_XPATH_LEN);
		if (agt_get_updated_param(child, parent_xpath, param_category,
			packer, packer_ready,
			req_hdr, schema_id, rsp_buf, mgr_sock) != 0) {
			return -1;
		}
		// restore parent_xpath
		char *last_slash = strrchr(parent_xpath, '/');
		pl_assert(last_slash != NULL);
		*last_slash = '\0';
	}
	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_get_updated_param_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf,
		int *mgr_sock)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	uint8_t param_category = 0;
	int agt_ret;

	agt_init_buf(rsp_buf);
	//payload in request must
	if ((ret = devapi_msg_unpack_ptv_val(unpacker, "param_category",
			DEVAPI_DATATYPE_UINT8, &param_category)) != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to unpack param_category, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Unpacking param_category", req_hdr, rsp_buf);
		return 0;
	}
	PL_LOG_VERBOSE("param_category: %u", param_category);

	const struct dm_schema_node *node = dm_get_root_node(&g_dm, req_hdr->target_schema);
	char parent_xpath[DEVAPI_XPATH_LEN];
	memset(parent_xpath, 0, DEVAPI_XPATH_LEN);
	bool packer_ready = false;
	agt_ret = agt_get_updated_param(node, parent_xpath, param_category,
			&packer, &packer_ready,
			req_hdr, schema_id, rsp_buf, mgr_sock);
	if (agt_ret == -1) {
		return -1;
	} else if (agt_ret == 1) {
		// Error message ready
		return 0;
	} else if (agt_ret == 0) {
		if (packer_ready) {
			// Some response ready
		} else {
			PL_LOG_DEBUG("Need to send empty response for GET_UPDATED_PARAM");
			devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
			packer_ready = true;
			ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
					DEVAPI_GET_UPDATED_PARAM_RESP, req_hdr->target_schema,
					g_dm.schema_id);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
			ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
			uint16_t num_of_objs = 0;
			ret = devapi_msg_pack_ptv(&packer, "num_of_objs",
					DEVAPI_DATATYPE_UINT16, &num_of_objs);
			if (ret != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to pack num_of_objs 0, ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
						req_hdr, rsp_buf);
				return 0;
			}
		}
	} else {
		// Does not reach here
	}

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_tx_start_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	enum devapi_rsp_status rsp_status = DEVAPI_STATUS_SUCCESS;
	char *err_msg = NULL;
	struct devapi_msg_packer_ctx packer;

	agt_init_buf(rsp_buf);

	// No payload required in tx-start request
	if (req_hdr->msg_len > (sizeof(struct devapi_msg_hdr)
			+ strlen(schema_id) + 1)) {
		PL_LOG_INFO("Ignoring the payload received");
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_TX_START_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack header, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	if (g_agt.in_trans) {
		rsp_status = DEVAPI_STATUS_APP_ERR;
		err_msg = "Transaction already in progress";
	}
	ret = devapi_msg_pack_status(&packer, rsp_status, err_msg);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
	}

	g_agt.in_trans = true;

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_set_config_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	uint16_t num_objs = 0;
	uint16_t num_params = 0;
	char obj_id[128];
	char param_id[16];
	int obj_iter = 0;
	int param_iter = 0;

	agt_init_buf(rsp_buf);
	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_SET_CONFIG_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		agt_rollback(req_hdr->target_schema);
		return 0;
	}

	// payload in request must
	if ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_objs",
		DEVAPI_DATATYPE_UINT16, &num_objs)) != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to unpack payload, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Unpacking num_of_objs", req_hdr, rsp_buf);
		agt_rollback(req_hdr->target_schema);
		return 0;
	}
	PL_LOG_VERBOSE("num_objs: %u", num_objs);
	for (obj_iter = 0; obj_iter < num_objs; obj_iter++) {
		// unpack obj_id & num_of_params
		if (((ret = devapi_msg_unpack_field(unpacker, "obj_id",
			DEVAPI_DATATYPE_STRING128, obj_id)) != DEVAPI_SUCCESS) ||
		    ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_params",
			DEVAPI_DATATYPE_UINT16, &num_params)) != DEVAPI_SUCCESS)) {
			PL_LOG_ERROR("Failed to unpack obj_id/num_params,"
					" ret=%d", ret);
			agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
					"Failed to unpack obj_id/num_params",
					req_hdr, rsp_buf);
			agt_rollback(req_hdr->target_schema);
			return 0;
		}
		PL_LOG_VERBOSE("obj_id[%d]: %s, num_params[%d]",
				obj_iter+1, obj_id, num_params);
		const struct schema_node *obj = dm_get_node(&g_dm, req_hdr->target_schema, obj_id);
		if (obj == NULL) {
			PL_LOG_ERROR("Object %s not found in tree", obj_id);
			char err_msg[DEVAPI_ERR_MSG_LEN];
			pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s",
					"Bad object ", obj_id);
			agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
					req_hdr, rsp_buf);
			agt_rollback(req_hdr->target_schema);
			return 0;
		}
		//dm_print_param(&g_dm, obj, pl_printf, stdout);  // XXX: remove this line after debug

		for (param_iter = 0; param_iter < num_params; param_iter++) {
			// Unpack param
			char param_id[DEVAPI_DATATYPE_STRING16];
			enum devapi_datatype datatype;
			char val[DEVAPI_DATATYPE_STRING256];  // XXX: largest value
			if ((ret = devapi_msg_unpack_ptv(unpacker,
				param_id, sizeof(param_id), &datatype,
				(void *)val, sizeof(val))) != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to unpack param_id,"
						" ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
						"Failed to unpack param_id",
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}
			PL_LOG_VERBOSE("param_id[%d][%d]: %s %u",
					obj_iter+1, param_iter+1, param_id, datatype);
			const struct schema_node *param = dm_get_child_schema_node(obj, param_id, strlen(param_id));
			if (param == NULL) {
				PL_LOG_ERROR("Param %s/%s not found in tree",
						obj_id, param_id);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s",
						"Bad param ", obj_id, param_id);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}
			//dm_print_param(&g_dm, param, pl_printf, stdout);  // XXX: remove this line after debug
			if (datatype != param->datatype) {
				PL_LOG_ERROR("Param %s/%s datatype %u != received datatype %u",
						obj_id, param_id, param->datatype, datatype);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s expected datatype %u",
						"Bad param datatype ", obj_id, param_id, param->datatype);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}

			// TODO value size, syntax check
			if (dm_set_param_value(&g_dm, param, val) != 0) {
				PL_LOG_ERROR("Set failed for %s/%s", obj_id, param_id);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s",
						"Validation failed for ", obj_id, param_id);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}
		}
	}

	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		agt_rollback(req_hdr->target_schema);
	}

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_reset_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	uint16_t num_objs = 0;
	uint16_t num_params = 0;
	char obj_id[128];
	char param_id[16];
	int obj_iter = 0;
	int param_iter = 0;

	agt_init_buf(rsp_buf);
	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_RESET_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack msg hdr, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		agt_rollback(req_hdr->target_schema);
		return 0;
	}

	// payload in request must
	if ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_objs",
		DEVAPI_DATATYPE_UINT16, &num_objs)) != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to unpack payload, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Unpacking num_of_objs", req_hdr, rsp_buf);
		agt_rollback(req_hdr->target_schema);
		return 0;
	}
	PL_LOG_VERBOSE("num_objs: %u", num_objs);
	if (num_objs == 0) {
		// XXX: RESET config tree alone
		if (dm_reset_config_tree(&g_dm,
				dm_get_root_node(&g_dm, req_hdr->target_schema)) != 0) {
			PL_LOG_ERROR("Failed to reset the card");
			agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
					"Failed to reset",
					req_hdr, rsp_buf);
			agt_rollback(req_hdr->target_schema);
			return 0;
		}
	}
	for (obj_iter = 0; obj_iter < num_objs; obj_iter++) {
		// unpack obj_id & num_of_params
		if (((ret = devapi_msg_unpack_field(unpacker, "obj_id",
			DEVAPI_DATATYPE_STRING128, obj_id)) != DEVAPI_SUCCESS) ||
		    ((ret = devapi_msg_unpack_ptv_val(unpacker, "num_of_params",
			DEVAPI_DATATYPE_UINT16, &num_params)) != DEVAPI_SUCCESS)) {
			PL_LOG_ERROR("Failed to unpack obj_id/num_params,"
					" ret=%d", ret);
			agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
					"Failed to unpack obj_id/num_params",
					req_hdr, rsp_buf);
			agt_rollback(req_hdr->target_schema);
			return 0;
		}
		PL_LOG_VERBOSE("obj_id[%d]: %s, num_params[%d]",
				obj_iter+1, obj_id, num_params);
		const struct schema_node *obj = dm_get_node(&g_dm, req_hdr->target_schema, obj_id);
		if (obj == NULL) {
			PL_LOG_ERROR("Object %s not found in tree", obj_id);
			char err_msg[DEVAPI_ERR_MSG_LEN];
			pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s",
					"Bad object ", obj_id);
			agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
					req_hdr, rsp_buf);
			agt_rollback(req_hdr->target_schema);
			return 0;
		}
		//dm_print_param(&g_dm, obj, pl_printf, stdout);  // XXX: remove this line after debug
		if (num_params == 0) {
			if (dm_reset_config_tree(&g_dm, obj) != 0) {
				PL_LOG_ERROR("Failed to reset object %s", obj_id);
				agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
						"Failed to reset",
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}
		}

		for (param_iter = 0; param_iter < num_params; param_iter++) {
			// Unpack param
			if ((ret = devapi_msg_unpack_field(unpacker, "param_id",
				DEVAPI_DATATYPE_STRING16, param_id)) != DEVAPI_SUCCESS) {
				PL_LOG_ERROR("Failed to unpack param_id,"
						" ret=%d", ret);
				agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
						"Failed to unpack param_id",
						req_hdr, rsp_buf);
				return 0;
			}
			PL_LOG_VERBOSE("param_id[%d][%d]: %s",
					obj_iter+1, param_iter+1, param_id);
			const struct schema_node *param = dm_get_child_schema_node(obj, param_id, strlen(param_id));
			if (param == NULL) {
				PL_LOG_ERROR("Param %s/%s not found in tree",
						obj_id, param_id);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s",
						"Bad param ", obj_id, param_id);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}
			//dm_print_param(&g_dm, param, pl_printf, stdout);  // XXX: remove this line after debug
			if (dm_reset_param(&g_dm, param) != 0) {
				PL_LOG_ERROR("Reset failed for %s/%s", obj_id, param_id);
				char err_msg[DEVAPI_ERR_MSG_LEN];
				pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN, "%s%s/%s",
						"Reset failed for ", obj_id, param_id);
				agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
						req_hdr, rsp_buf);
				agt_rollback(req_hdr->target_schema);
				return 0;
			}
		}
	}

	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		agt_rollback(req_hdr->target_schema);
	}

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_tx_end_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	enum devapi_rsp_status rsp_status = DEVAPI_STATUS_SUCCESS;
	char *commit_err_msg = NULL;
	struct devapi_msg_packer_ctx packer;
	uint8_t commit = DEVAPI_COMMIT;
		PL_LOG_ERROR("Init value for commit %u", commit);

	agt_init_buf(rsp_buf);

	//payload in request must
	if ((ret = devapi_msg_unpack_ptv_val(unpacker, "commit",
		DEVAPI_DATATYPE_UINT8, &commit)) != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to unpack commit, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Unpacking num_of_objs", req_hdr, rsp_buf);
		return 0;
	}
		PL_LOG_ERROR("Ex value for commit %u", commit);
	if (commit != DEVAPI_COMMIT && commit != DEVAPI_ROLLBACK) {
		char err_msg[DEVAPI_ERR_MSG_LEN];
		PL_LOG_ERROR("Bad value for commit %u", commit);
		pl_snprintf(err_msg, DEVAPI_ERR_MSG_LEN,
				"Bad value for commit %u", commit);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, err_msg,
				req_hdr, rsp_buf);
		return 0;
	}
	if (!g_agt.in_trans) {
		PL_LOG_ERROR("No transaction in progress");
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
				"No transaction in progress", req_hdr, rsp_buf);
		return 0;
	}

	if (commit == DEVAPI_COMMIT) {
		// DO COMMIT
		if (agt_commit(req_hdr->target_schema) != 0) {
			rsp_status = DEVAPI_STATUS_APP_ERR;
			commit_err_msg = "Commit failed";
		} else {
			rsp_status = DEVAPI_STATUS_SUCCESS;
			commit_err_msg = NULL;
		}
	} else {
		// DO ROLLBACK
		if (agt_rollback(req_hdr->target_schema) != 0) {
			rsp_status = DEVAPI_STATUS_APP_ERR;
			commit_err_msg = "Commit failed";
		} else {
			rsp_status = DEVAPI_STATUS_SUCCESS;
			commit_err_msg = NULL;
		}
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_TX_END_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack header, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		// TODO: Commit is done but response failed
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, rsp_status, commit_err_msg);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		// TODO: Commit is done but response failed
	}

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_reboot_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	enum devapi_rsp_status rsp_status = DEVAPI_STATUS_SUCCESS;
	char *err_msg = NULL;
	struct devapi_msg_packer_ctx packer;

	agt_init_buf(rsp_buf);

	// No payload required in reboot request
	if (req_hdr->msg_len > (sizeof(struct devapi_msg_hdr)
			+ strlen(schema_id) + 1)) {
		PL_LOG_INFO("Ignoring the payload received");
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_REBOOT_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack header, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}

	ret = devapi_msg_pack_status(&packer, rsp_status, err_msg);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
	}

	ret = agt_send_msg(rsp_buf, &g_agt.mgr_sock);
	if (ret != 0) {
		PL_LOG_ERROR("agt_send_msg failed, ret=%d", ret);
	}


	do_reboot();

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_sw_image_download_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	struct pl_tftp_server srv;

	agt_init_buf(rsp_buf);

	if (((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_srv_ip",
		DEVAPI_DATATYPE_STRING16, srv.ipaddr)) != DEVAPI_SUCCESS) ||
	    ((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_srv_port",
		DEVAPI_DATATYPE_UINT16, &srv.port)) != DEVAPI_SUCCESS) ||
	    ((ret = devapi_msg_unpack_ptv_val(unpacker, "tftp_path",
		DEVAPI_DATATYPE_STRING128, srv.path)) != DEVAPI_SUCCESS)) {
		PL_LOG_ERROR("Failed to get TFTP params, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Failed to read payload", req_hdr, rsp_buf);
		return 0;
	}
	if ((ret = pl_download_sw_image(&srv)) != 0) {
		PL_LOG_ERROR("Failed to download SW image files, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
				"Failed to download SW image files",
				req_hdr, rsp_buf);
		return 0;
	}

	g_agt.sw_update_state = DEVAPI_SU_STATUS_DOWNLOAD_SUCCESS;

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_SW_IMAGE_DOWNLOAD_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack msghdr, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
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
agt_process_sw_update_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	struct pl_tftp_server srv;

	agt_init_buf(rsp_buf);

	// No payload required in the request
	if (req_hdr->msg_len > (sizeof(struct devapi_msg_hdr)
			+ strlen(schema_id) + 1)) {
		PL_LOG_INFO("Ignoring the payload received");
	}

	if ((ret = pl_update_sw(&srv)) != 0) {
		PL_LOG_ERROR("Failed to update SW, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
				"Failed to update SW",
				req_hdr, rsp_buf);
		return 0;
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_SW_UPDATE_RESP, req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack msghdr, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}

	ret = agt_send_msg(rsp_buf, &g_agt.mgr_sock);
	if (ret != 0) {
		PL_LOG_ERROR("agt_send_msg failed, ret=%d", ret);
	}

	do_reboot();

	return 0;
}

/*
 * returns 0 if successful (even if input request processing failed)
 * return -1 only in case of abnormal situation.
 */
static int
agt_process_get_sw_update_status_req(
		struct devapi_msg_packer_ctx *unpacker,
		const struct devapi_msg_hdr *req_hdr,
		const char *schema_id,
		struct agt_buf *rsp_buf)
{
	enum devapi_return ret;
	struct devapi_msg_packer_ctx packer;
	struct pl_tftp_server srv;

	agt_init_buf(rsp_buf);

	// No payload required in the request
	if (req_hdr->msg_len > (sizeof(struct devapi_msg_hdr)
			+ strlen(schema_id) + 1)) {
		PL_LOG_INFO("Ignoring the payload received");
	}

	if ((ret = pl_get_sw_update_status(&srv)) != 0) {
		PL_LOG_ERROR("Failed to get SW update status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR,
				"Failed to get SW update status",
				req_hdr, rsp_buf);
		return 0;
	}

	devapi_msg_packer_init(&packer, rsp_buf->buf, sizeof(rsp_buf->buf));
	ret = devapi_msg_pack_msghdr(&packer, req_hdr->msg_id,
			DEVAPI_GET_SW_UPDATE_STATUS_RESP,
			req_hdr->target_schema,
			g_dm.schema_id);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack msghdr, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	ret = devapi_msg_pack_status(&packer, DEVAPI_STATUS_SUCCESS, NULL);
	if (ret != DEVAPI_SUCCESS) {
		PL_LOG_ERROR("Failed to pack status, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
				req_hdr, rsp_buf);
		return 0;
	}
	if (((ret = devapi_msg_pack_ptv(&packer, "update_status",
			DEVAPI_DATATYPE_UINT8, &g_agt.sw_update_state))
			== DEVAPI_SUCCESS) &&
		((ret = devapi_msg_pack_ptv(&packer, "update_error",
			DEVAPI_DATATYPE_STRING256, ""))
			== DEVAPI_SUCCESS)) {
		// pass
	} else {
		PL_LOG_ERROR("Failed to pack PTV, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_APP_ERR, "Internal error",
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
agt_process_req(const struct agt_buf *req_buf, int *mgr_sock,
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
		PL_LOG_ERROR("Unpack msghdr failed, ret=%d", ret);
		agt_make_err_rsp(DEVAPI_STATUS_MSG_FORMAT_ERR,
				"Failed to read msghdr", &req_hdr, rsp_buf);
		return 0;
	}
	agt_dump_msg_hdr(&req_hdr, schema_id);

	agt_init_buf(rsp_buf);

	rsp_status = agt_validate_req(&req_hdr, schema_id, &err_msg);
	if (rsp_status != DEVAPI_STATUS_SUCCESS) {
		agt_make_err_rsp(rsp_status, err_msg, &req_hdr, rsp_buf);
		return 0;
	}

	switch (msg->hdr.msg_type) {
	case DEVAPI_HELLO_REQ:
		ret = agt_process_hello_req(&unpacker, &req_hdr, schema_id,
				rsp_buf);
		break;
	case DEVAPI_GET_SCHEMA_REQ:
		ret = agt_process_get_schema_req(&unpacker, &req_hdr, schema_id,
				rsp_buf);
		break;
	case DEVAPI_GET_PARAM_REQ:
		ret = agt_process_get_param_req(&unpacker, &req_hdr, schema_id,
				rsp_buf, mgr_sock);
		break;
	case DEVAPI_GET_UPDATED_PARAM_REQ:
		ret = agt_process_get_updated_param_req(&unpacker, &req_hdr, schema_id,
				rsp_buf, mgr_sock);
		break;
		break;
	case DEVAPI_TX_START_REQ:
		ret = agt_process_tx_start_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_SET_CONFIG_REQ:
		ret = agt_process_set_config_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_TX_END_REQ:
		ret = agt_process_tx_end_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_REBOOT_REQ:
		ret = agt_process_reboot_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_RESET_REQ:
		ret = agt_process_reset_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_SW_IMAGE_DOWNLOAD_REQ:
		ret = agt_process_sw_image_download_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_SW_UPDATE_REQ:
		ret = agt_process_sw_update_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
		break;
	case DEVAPI_GET_SW_UPDATE_STATUS_REQ:
		ret = agt_process_get_sw_update_status_req(&unpacker, &req_hdr,
				schema_id, rsp_buf);
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
agt_read_msg(int fd, struct agt_buf *r_buf)
{
	static int nbytes = 0;
	size_t pending;
	bool big_msg = false;

	const struct devapi_msg_hdr *hdr = (struct devapi_msg_hdr *)r_buf->buf;
	if (r_buf->filled == 0) {
		pl_assert(hdr->msg_len == 0);
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
			PL_LOG_ERROR("Caller did not reset buffer");
			return -1;
		}
	}

	if (!big_msg) {
		nbytes = pl_recv(fd, (r_buf->buf + r_buf->filled), pending, 0);
	} else {
		// Let us drain the socket, let us keep overwriting
		// the schema_id + payload portion
		nbytes = pl_recv(fd, (r_buf->buf + sizeof(*hdr)), pending, 0);
	}
	if (nbytes < 0) {
		PL_LOG_ERROR("Read(fd=%d) failed, pl_errno=%d", fd, pl_errno);
		return -1;
	} else if (nbytes == 0) {
		PL_LOG_INFO("End-of-file for %d", fd);
		return -1;
	}
	r_buf->filled += nbytes;
	if (r_buf->filled < sizeof(hdr->msg_len)) {
		PL_LOG_VERBOSE("Read %d bytes, fd %d, total %d",
				nbytes, fd, r_buf->filled);
		return 0;
	}
	if (r_buf->filled < be16toh(hdr->msg_len)) {
		PL_LOG_VERBOSE("Read %d bytes, fd %d, total %d, pending %d",
				nbytes, fd, r_buf->filled,
				(be16toh(hdr->msg_len) - r_buf->filled));
		return 0; /* continue reading */
	} else {
		PL_LOG_VERBOSE("Read %d bytes from fd %d, msg_len %d",
				nbytes, fd, be16toh(hdr->msg_len));
		PL_LOG_INFO("Recevied msg %d of length %d from fd %d",
				hdr->msg_type,
				be16toh(hdr->msg_len), fd);
	}
	return 0;
}

void
agt_run_agent(int listen_sock, struct agt_buf *req_buf, struct agt_buf *rsp_buf)
{
	int ret = -1;

	g_agt.mgr_sock = -1;
	g_agt.in_trans = false;
	g_agt.sw_update_state = DEVAPI_SU_STATUS_IDLE;
	agt_init_buf(req_buf);
	agt_init_buf(rsp_buf);

	while (1) {
		bool read_ready = false;
		int mgr_sock_dup = g_agt.mgr_sock;
		if (pl_select_one(listen_sock, &mgr_sock_dup,
				true, &read_ready) != 0) {
			PL_LOG_ERROR("select failed. Exiting the loop");
			return;
		}
		if (mgr_sock_dup != g_agt.mgr_sock) {
			if (g_agt.mgr_sock == -1) {
				PL_LOG_INFO("New conn %d", mgr_sock_dup);
			} else {
				PL_LOG_INFO("Old conn [%d], Second conn [%d]",
					g_agt.mgr_sock, mgr_sock_dup);
				PL_LOG_INFO("Closing conn %d", g_agt.mgr_sock);
				pl_close(g_agt.mgr_sock);
				g_agt.mgr_sock = -1;
				// Close transaction and rollback
				if (g_agt.in_trans == true) {
					agt_rollback(DM_TYPE_MGMT);
				}
			}
			g_agt.mgr_sock = mgr_sock_dup;
			agt_init_buf(req_buf);
		} else if (read_ready) {
			// Read message
			if (agt_read_msg(g_agt.mgr_sock, req_buf) == -1) {
				PL_LOG_INFO("Closing conn %d", g_agt.mgr_sock);
				pl_close(g_agt.mgr_sock);
				g_agt.mgr_sock = -1;
				// Close transaction and rollback
				if (g_agt.in_trans == true) {
					agt_rollback(DM_TYPE_MGMT);
				}
			}
			if (!agt_is_req_msg_complete(req_buf)) {
				continue;
			}
			// Process request
			agt_init_buf(rsp_buf);
			ret = agt_process_req(req_buf, &g_agt.mgr_sock, rsp_buf);
			agt_init_buf(req_buf);
			if (ret != 0) {
				PL_LOG_ERROR("agt_process_req failed, ret=%d", ret);
				continue;
			}
			// Connection might have been closed
			if (g_agt.mgr_sock == -1) {
				PL_LOG_INFO("No connection to manager");
				continue;
			}
			// Send response
			ret = agt_send_msg(rsp_buf, &g_agt.mgr_sock);
			if (ret != 0) {
				PL_LOG_ERROR("agt_send_msg failed, ret=%d", ret);
				continue;
			}
		}
	}
}
