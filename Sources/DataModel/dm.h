#ifndef _DM_H_
#define _DM_H_

#include "dm_types.h"
#include <stdbool.h>

enum dm_type {
	DM_TYPE_NONE = 0,
	DM_TYPE_MGMT = 1,
	DM_TYPE_CALIBRATION = 2
};

/*
 * Initialize data model lock
 * Returns 0 on success; -1 on failure.
 */
int dm_lock_init(void);
/*
 * Lock the data model. Reentrancy not supported.
 * Returns 0 on success; -1 on failure to acquire lock
 */
int dm_lock(void);
/*
 * Unlock the data model. Reentrancy not supported.
 * Returns 0 on success; -1 on failure to release lock
 */
int dm_unlock(void);

/*
 * return 0 on success; -1 on failure
 */
int dm_get_param_value(const struct schema_node *node, void *value);
/*
 * return 0 on success; -1 on failure
 */
int dm_set_param_value(const struct schema_node *node, const void *value);
int dm_set_param_value_x(enum dm_type dm_type, const char *xpath, void *value);
/*
 * return 0 on success; -1 on failure
 */
int dm_reset_param(const struct schema_node *node);
/*
 * return 0 on success; -1 on failure
 */
int dm_reset_tree(const struct schema_node *node);

int __dm_print_param(const struct schema_node *node, int (*dm_printf)(void* priv, char *str, ...), void *priv, int tabs);
int __dm_print_param_x(enum dm_type dm_type, const char *xpath, int (*dm_printf)(void* priv, char *str, ...), void *priv, int tabs);
int __dm_print_tree(const struct schema_node *node, int (*dm_printf)(void* priv, char *str, ...), void *priv, int tabs);

#define dm_print_param(node, dm_printf, priv) __dm_print_param(node, dm_printf, priv, 0)
#define dm_print_param_x(node, dm_printf, priv) __dm_print_param_x(node, dm_printf, priv, 0)
#define dm_print_tree(node, dm_printf, priv) __dm_print_tree(node, dm_printf, priv, 0)

const struct schema_node *get_schema_node(const struct schema_node *root,
		const char *xpath);

const struct schema_node *dm_get_node(enum dm_type dm_type, const char *xpath);
const struct schema_node *get_child_schema_node(
		const struct schema_node *parent,
		const char *label, size_t label_len);

/*
 * returns 0 on success; -1 on error
 */
int get_datatype_size(uint8_t datatype, size_t *size, bool *is_str);

#endif /* _DM_H_ */
