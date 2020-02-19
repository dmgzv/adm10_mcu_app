#ifndef _DEVAPI_H_
#define _DEVAPI_H_

#include <stdint.h>
#include <string.h>

#define DEVAPI_SCHEMA_ID_LEN  (63+1)  // including null terminator
#define DEVAPI_XPATH_LEN      (255+1)  // including null terminator
#define DEVAPI_ERR_MSG_LEN    (255+1)  // including null terminator

enum devapi_msg_type {
	DEVAPI_MSG_INVALID = 0,
	DEVAPI_UNKNOWN_MSG = 1,
	DEVAPI_HELLO_REQ = 2,
	DEVAPI_HELLO_RESP = 3,
	DEVAPI_GET_SCHEMA_REQ = 4,
	DEVAPI_GET_SCHEMA_RESP = 5,
	DEVAPI_GET_PARAM_REQ = 6,
	DEVAPI_GET_PARAM_RESP = 7,
	DEVAPI_GET_UPDATED_PARAM_REQ = 8,
	DEVAPI_GET_UPDATED_PARAM_RESP = 9,
	DEVAPI_TX_START_REQ = 10,
	DEVAPI_TX_START_RESP = 11,
	DEVAPI_SET_CONFIG_REQ = 12,
	DEVAPI_SET_CONFIG_RESP = 13,
	DEVAPI_TX_END_REQ = 14,
	DEVAPI_TX_END_RESP = 15,
	DEVAPI_REBOOT_REQ = 16,
	DEVAPI_REBOOT_RESP = 17,
	DEVAPI_RESET_REQ = 18,
	DEVAPI_RESET_RESP = 19,
	DEVAPI_SW_IMAGE_DOWNLOAD_REQ = 20,
	DEVAPI_SW_IMAGE_DOWNLOAD_RESP = 21,
	DEVAPI_SW_UPDATE_REQ = 22,
	DEVAPI_SW_UPDATE_RESP = 23,
	DEVAPI_GET_SW_UPDATE_STATUS_REQ = 24,
	DEVAPI_GET_SW_UPDATE_STATUS_RESP = 25,
	DEVAPI_MSG_MAX
};

enum devapi_datatype {
	DEVAPI_DATATYPE_INVALID = 0,
	DEVAPI_DATATYPE_BOOL = 1,
	DEVAPI_DATATYPE_ENUM = 2,
	DEVAPI_DATATYPE_INT8 = 3,		// In network byte order
	DEVAPI_DATATYPE_UINT8 = 4,		// In network byte order
	DEVAPI_DATATYPE_INT16 = 5,		// In network byte order
	DEVAPI_DATATYPE_UINT16 = 6,		// In network byte order
	DEVAPI_DATATYPE_INT32 = 7,		// In network byte order
	DEVAPI_DATATYPE_UINT32 = 8,		// In network byte order
	DEVAPI_DATATYPE_INT64 = 9,		// In network byte order
	DEVAPI_DATATYPE_UINT64 = 10,		// In network byte order
	DEVAPI_DATATYPE_FLOAT1 = 11,		// Float with 1 fraction digit
	DEVAPI_DATATYPE_FLOAT2 = 12,		// Float with 2 fraction digits
	DEVAPI_DATATYPE_FLOAT3 = 13,		// Float with 3 fraction digits
	DEVAPI_DATATYPE_FLOATE1 = 14, 	// Float with format: n.ne+nn
	DEVAPI_DATATYPE_FLOATE18 = 15,	// Float with format: n.ne-nn
	DEVAPI_DATATYPE_STRING16 = 16,	// String size 16 (includes null char)
	DEVAPI_DATATYPE_STRING32 = 17,	// String size 32 (includes null char)
	DEVAPI_DATATYPE_STRING64 = 18,	// String size 64 (includes null char)
	DEVAPI_DATATYPE_STRING128 = 19,	// String size 128 (includes null char)
	DEVAPI_DATATYPE_STRING256 = 20,	// String size 256 (includes null char)
};

enum devapi_param_category {
	DEVAPI_PARAMCAT_INVALID = 0,
	DEVAPI_PARAMCAT_INVENTORY = 1,
	DEVAPI_PARAMCAT_CONFIG = 2,
	DEVAPI_PARAMCAT_MONITOR = 3,
	DEVAPI_PARAMCAT_FAULT = 4,
	DEVAPI_PARAMCAT_PERFORMANCE = 5,
	DEVAPI_PARAMCAT_ATTRIBUTE = 6
};

enum devapi_card_type {
	DEVAPI_HALF_SLOT = 1,
	DEVAPI_FULL_SLOT = 2,
	DEVAPI_DOUBLE_SLOT = 3
};

// Same as enum dm_type in dm.h
enum devapi_schema_type {
	DEVAPI_SCHEMA_CARDMGMT = 0,
	DEVAPI_SCHEMA_CALIBRATION = 1
};

enum devapi_notation {
	DEVAPI_NOTATION_XPATH = 0,
	DEVAPI_NOTATION_OID = 1
};

enum devapi_return {
	DEVAPI_SUCCESS = 0,
	//General errors
	DEVAPI_INVALID_PARAM = 10,
	DEVAPI_INVALID_MSGTYPE = 11,
	DEVAPI_DATATYPE_MISMATCH = 12,
	DEVAPI_MEMORY_FAILURE = 13,
	DEVAPI_FAILURE = 14,
	// Packer errors
	DEVAPI_PACKER_NO_INIT = 20,
	DEVAPI_PACKER_NO_HEADER = 21,
	DEVAPI_PACKER_NO_OBJ_HDR = 22,
	DEVAPI_PACKER_NO_MEM = 23,
	DEVAPI_PACKER_FAILURE = 24
};

enum devapi_rsp_status {
	DEVAPI_STATUS_SUCCESS = 0,
	DEVAPI_STATUS_MSG_FORMAT_ERR = 1,
	DEVAPI_STATUS_REQ_UNSUPPORTED = 2,
	DEVAPI_STATUS_SCHEMA_ERR = 3,
	DEVAPI_STATUS_APP_ERR = 4
};

enum devapi_sw_update_status {
	DEVAPI_SU_STATUS_IDLE = 0,
	DEVAPI_SU_STATUS_DOWNLOAD_FAILED = 1,
	DEVAPI_SU_STATUS_DOWNLOAD_SUCCESS = 2,
	DEVAPI_SU_STATUS_UPDATE_FAILED = 3
};

enum devapi_commit_opt {
	DEVAPI_COMMIT = 0,
	DEVAPI_ROLLBACK = 1
};

struct __attribute__((__packed__)) devapi_msg_hdr {
	uint16_t msg_len;
	uint32_t msg_id;
	uint8_t  msg_type;
	uint8_t  target_schema; // 0=card-mgmt-schema, 1=calibration-schema
	uint8_t  id_notation;   // 0=XPATH, 1=OID
	uint8_t  msg_last_fragment;
	// uint8_t  schema_id[DEVAPI_SCHEMA_ID_LEN];
	// XXX: schema_id is of dynamic size in the message
};

struct devapi_req_result {
	enum devapi_rsp_status status;
	uint8_t error_msg[DEVAPI_ERR_MSG_LEN];
};

const char* devapi_return_str(enum devapi_return ret);
const char* devapi_rsp_status_str(enum devapi_rsp_status rsp);
const char* devapi_sw_update_status_str(enum devapi_sw_update_status stat);
const char* devapi_msg_type_str(enum devapi_msg_type msg);
const char* devapi_datatype_str(enum devapi_datatype type);
const char* devapi_param_category_str(enum devapi_param_category pc);
const char* devapi_commit_opt_str(enum devapi_commit_opt co);
const char* devapi_card_type_str(enum devapi_card_type cardtype);

#endif

