#ifndef _DM_H_
#define _DM_H_

#include <stdbool.h>
#include "dm_types.h"
#include "pl.h"

#define dm_schema_node schema_node //FIXME

enum dm_type {
	DM_TYPE_MGMT = 0,        // Same as DEVAPI_SCHEMA_CARDMGMT
	DM_TYPE_CALIBRATION = 1  // Same as DEVAPI_SCHEMA_CALIBRATION
};

enum dm_card_size {
	DM_HALF_SLOT = 1,
	DM_FULL_SLOT = 2,
	DM_DOUBLE_SLOT = 3
};

struct dm_data_blk {
	void *values;
	uint32_t values_size;
};

struct dm_schema {
	const struct dm_schema_node *root;
	struct dm_data_blk attribute;
	struct dm_data_blk config;
	struct dm_data_blk inventory;
	struct dm_data_blk monitor;
	struct dm_data_blk fault;
	struct dm_data_blk gauge;
	struct dm_data_blk counter;
};

struct dm {
	dm_mutex_id_t mutex_id;
	enum dm_card_size card_size;
	const char *schema_id;
	struct dm_schema *mgmt;
	struct dm_schema *calib;
};


typedef int (*dm_printf_funcp)(void *pr_handle, const char *fmt, ...);

int dm_init(struct dm *dm, enum dm_card_size card_size, const char *schema_id,
		struct dm_schema *mgmt, struct dm_schema *calib);

/*
 * Initialize data model lock
 * Returns 0 on success; -1 on failure.
 */
int dm_lock_init(dm_mutex_id_t *mutex);
/*
 * Lock the data model. Reentrancy not supported.
 * Returns 0 on success; -1 on failure to acquire lock
 */
int dm_lock(dm_mutex_id_t *mutex);
/*
 * Unlock the data model. Reentrancy not supported.
 * Returns 0 on success; -1 on failure to release lock
 */
int dm_unlock(dm_mutex_id_t *mutex);

/*
 * return 0 on success; -1 on failure
 */
int dm_get_param_value(struct dm *dm,
		const struct dm_schema_node *node, void *value);
/*
 * return 0 on success; -1 on failure
 */
int dm_set_param_value(struct dm *dm,
		const struct dm_schema_node *node, const void *value);
int dm_set_param_value_x(struct dm *dm, enum dm_type dm_type,
		const char *xpath, void *value);
/*
 * return 0 on success; -1 on failure
 */
int dm_reset_param(struct dm *dm, const struct dm_schema_node *node);
/*
 * return 0 on success; -1 on failure
 */
int dm_reset_tree(struct dm *dm, const struct dm_schema_node *node);
/*
 * return 0 on success; -1 on failure
 */
int dm_reset_config_tree(struct dm *dm, const struct dm_schema_node *node);

int dm_print_param(struct dm *dm, const struct dm_schema_node *node,
		dm_printf_funcp pr_funp, void *pr_handle);
int dm_print_param_x(struct dm *dm, enum dm_type dm_type, const char *xpath,
		dm_printf_funcp pr_funp, void *pr_handle);
int dm_print_tree(struct dm *dm, const struct dm_schema_node *node,
		dm_printf_funcp pr_funp, void *pr_handle);

const struct dm_schema_node *dm_get_root_node(struct dm *dm,
		enum dm_type dm_type);
const struct dm_schema_node *dm_get_schema_node(
		const struct dm_schema_node *root, const char *xpath);

const struct dm_schema_node *dm_get_node(struct dm *dm, enum dm_type dm_type,
		const char *xpath);
const struct dm_schema_node *dm_get_child_schema_node(
		const struct dm_schema_node *parent,
		const char *label, size_t label_len);

/*
 * returns 0 on success; -1 on error
 */
int dm_get_datatype_size(uint8_t datatype, size_t *size, bool *is_str);

int dm_load_startup_config(const char *dm_schema_id, enum dm_type dm_type,
		void *mgmt_cfg, size_t cmgmt_fg_len,
		void *clbr_cfg, size_t clbr_cfg_len);
int dm_write_startup_config(const char *dm_schema_id, enum dm_type dm_type,
		const void *mgmt_cfg, size_t mgmt_cfg_len,
		const void *clbr_cfg, size_t clbr_cfg_len);

#endif /* _DM_H_ */
