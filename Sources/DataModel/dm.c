#include <inttypes.h>

#include "pl.h"
#include "dm.h"

const struct dm_schema_node *
dm_get_root_node(struct dm *dm, enum dm_type dm_type)
{
	switch (dm_type) {
	case DM_TYPE_MGMT:
		return dm->mgmt->root;
	case DM_TYPE_CALIBRATION:
		return dm->calib->root;
	}
	return NULL;
}

int
dm_init(struct dm *dm, enum dm_card_size card_size, const char *schema_id,
		struct dm_schema *mgmt, struct dm_schema *calib)
{
	memset(dm, 0, sizeof(*dm));
	dm->card_size = card_size;
	dm->schema_id = schema_id;
	dm->mgmt = mgmt;
	dm->calib = calib;

	if (dm_lock_init(&dm->mutex_id) != 0) {
		PL_LOG_ERROR("Failed to init data model lock");
		return -1;
	}

	// First initialize with known defaults
	PL_LOG_INFO("Resetting MGMT tree");
	if (dm_reset_tree(dm, dm_get_root_node(dm, DM_TYPE_MGMT)) != 0) {
		PL_LOG_ERROR("Failed resetting MGMT tree");
		return -1;
	}
	PL_LOG_INFO("Resetting CALIBRATION tree");
	if (dm_reset_tree(dm, dm_get_root_node(dm, DM_TYPE_CALIBRATION)) != 0) {
		PL_LOG_ERROR("Failed resetting CALIBRATION tree");
		return -1;
	}
	// Load startup config
	if (dm_load_startup_config(schema_id, DM_TYPE_MGMT,
				dm->mgmt->config.values,
				dm->mgmt->config.values_size,
				dm->calib->config.values,
				dm->calib->config.values_size) != 0) {
		PL_LOG_ERROR("Failed to load startup config");
	}

	return 0;
}

const struct dm_schema_node *
dm_get_child_schema_node(const struct dm_schema_node *parent,
		const char *label, size_t label_len)
{
	int i;

	pl_assert(parent != NULL);
	pl_assert(label != NULL);

	if (parent->category == SCHEMA_NODE_TYPE_ROOT ||
			parent->category == SCHEMA_NODE_TYPE_OBJECT) {
		for (i = 0; i < parent->num_children; i++) {
			const struct dm_schema_node *ch =
				((struct dm_schema_node **)parent->val)[i];
			if (strncmp(ch->label, label, label_len) == 0) {
				if (ch->label[label_len] != '\0') {
					PL_LOG_DEBUG("Partial match in %s len:%d",
							ch->label, label_len);
					continue;
				}
				return ch;
			}
		}
	}
	return NULL;
}

const struct dm_schema_node *
dm_get_schema_node(const struct dm_schema_node *root, const char *xpath)
{
	const struct dm_schema_node *sn = root;
	const char *sep_p = NULL;
	const char *xp2 = xpath;

	pl_assert(root != NULL);
	pl_assert(xpath != NULL);
	sep_p = strchr(xp2, '/');
	if (sep_p != xp2) {
		PL_LOG_DEBUG("Bad xpath %s", xpath);
		return NULL;
	}
	while (sep_p != NULL) {
		xp2 = sep_p + 1;
		sep_p = strchr(xp2, '/');
		if (sn->category == SCHEMA_NODE_TYPE_ROOT ||
				sn->category == SCHEMA_NODE_TYPE_OBJECT) {
			// PL_LOG_VERBOSE("OBJECT %s in %s", label, xpath);
			sn = dm_get_child_schema_node(sn, xp2,
					(sep_p ? (sep_p - xp2) : strlen(xp2)));
			if (sn == NULL) {
				PL_LOG_DEBUG("'%s' not in XPATH %s",
						(xp2-1), xpath);
				return NULL;
			} else if (sep_p == NULL) {
				return sn;
			}
		} else {
			PL_LOG_DEBUG("'%s' not in XPATH %s", (xp2-1), xpath);
			return NULL;
		}
	}
	return NULL;
}

const struct dm_schema_node *
dm_get_node(struct dm *dm, enum dm_type dm_type, const char *xpath)
{
	pl_assert(xpath != NULL);
	const struct dm_schema_node *sn = dm_get_root_node(dm, dm_type);
	return dm_get_schema_node(sn, xpath);
}

/*
 * returns 0 on success; -1 on error
 */
int
dm_get_datatype_size(uint8_t datatype, size_t *size, bool *is_str)
{
	switch (datatype) {
	case SCHEMA_DATATYPE_BOOL:
		*size = 1;
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_ENUM:
		*size = 4;
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_INT8:
	case SCHEMA_DATATYPE_UINT8:
		*size = 1;
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_INT16:
	case SCHEMA_DATATYPE_UINT16:
		*size = 2;
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_INT32:
	case SCHEMA_DATATYPE_UINT32:
		*size = 4;
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_INT64:
	case SCHEMA_DATATYPE_UINT64:
		*size = 8;
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_FLOAT_1:
	case SCHEMA_DATATYPE_FLOAT_2:
	case SCHEMA_DATATYPE_FLOAT_3:
	case SCHEMA_DATATYPE_FLOAT_E1:
	case SCHEMA_DATATYPE_FLOAT_E18:
		*size = sizeof(float);
		*is_str = false;
		break;
	case SCHEMA_DATATYPE_STRING_16:
		*size = 16;
		*is_str = true;
		break;
	case SCHEMA_DATATYPE_STRING_32:
		*size = 32;
		*is_str = true;
		break;
	case SCHEMA_DATATYPE_STRING_64:
		*size = 64;
		*is_str = true;
		break;
	case SCHEMA_DATATYPE_STRING_128:
		*size = 128;
		*is_str = true;
		break;
	case SCHEMA_DATATYPE_STRING_256:
		*size = 256;
		*is_str = true;
		break;
	default:
		PL_LOG_ERROR("Unknown datatype %d", datatype);
		return -1;
	}
	return 0;
}

/*
 * return 0 on success; -1 on failure
 */
int
dm_get_param_value(struct dm *dm, const struct dm_schema_node *node, void *val)
{
	int ret = -1;
	int ret2 = -1;
	size_t datatype_size = 0;
	bool is_str = false;

	if ((!node) || (!val))
		return 0;

	if ((ret = dm_lock(&dm->mutex_id)) != 0) {
		PL_LOG_ERROR("Failed to lock DM");
		return -1;
	}

	if (dm_get_datatype_size(node->datatype, &datatype_size, &is_str) != 0) {
		PL_LOG_ERROR("Unknown datatype %d", node->datatype);
		ret = -1;
	} else {
		if (is_str) {
			if (strlcpy(val, node->val, datatype_size) >=
					datatype_size) {
				PL_LOG_ERROR("array_size %d is short",
						datatype_size);
			}
		} else {
			memcpy(val, node->val, datatype_size);
		}
		ret = 0;
	}

	if ((ret2 = dm_unlock(&dm->mutex_id)) != 0) {
		PL_LOG_ERROR("Failed to unlock DM");
		ret = -1;
	}
	return ret;
}

#define VALIDATE_RANGE_FIXED(type, val_p, range_p) \
	((*(type*)(val_p) < ((type*)(range_p))[0]) || \
			(*(type*)(val_p) > ((type*)(range_p))[1]))

#define VALIDATE_RANGE_REF(type, val_p, range_ref_p) \
	((*(type*)(val_p) < *((type**)(range_ref_p))[0]) || \
			(*(type*)(val_p) > *((type**)(range_ref_p))[1]))

#define VALIDATE_LOG_ERR_FIXED(type, fmt, val_p, range_p) \
	PL_LOG_ERROR("Set validation error- " fmt " not in [" fmt " .. " fmt "]",\
			*(type*)(val_p), ((type*)(range_p))[0], \
			((type*)(range_p))[1])

#define VALIDATE_LOG_ERR_REF(type, fmt, val_p, range_ref_p) \
	PL_LOG_ERROR("Set validation error - " fmt " not in [" fmt " .. " fmt "]",\
			*(type*)(val_p), *((type**)(range_ref_p))[0], \
			*((type**)(range_ref_p))[1])

/*
 * NOTE: This function does not lock DM.
 * return 0 on success; -1 on failure
 */
static int
dm_validate_set_value(const struct dm_schema_node *node, const void *val)
{
	int i = 0;
	if (node->category != SCHEMA_NODE_TYPE_CONFIG) {
		// Only CONFIG parameters updated using external Device API.
		// No validation required for other parameters.
		return 0;
	}
	switch (node->datatype) {
	case SCHEMA_DATATYPE_BOOL:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			// TODO: There could be range with only true or false
			return -1;
		case SCHEMA_NODE_RANGETYPE_REFS:
			// TODO: There could be range with only true or false
			return -1;
		default:
			if ((*(uint8_t*)val == 0) || (*(uint8_t*)val == 1)) {
				return 0;
			}
			PL_LOG_ERROR("%d is not bool value", *(uint8_t*)val);
			return -1;
		}
		return -1;
	case SCHEMA_DATATYPE_ENUM:
		for (i = 0; i < ((int32_t*)node->range)[0]; i++) {
			if (*((int32_t*)val) == ((int32_t*)node->range)[i+1]) {
				return 0;
			}
		}
		PL_LOG_ERROR("Enum %d is not in set ( ", *((int32_t*)val));
		for (i = 0; i < ((int32_t*)node->range)[0]; i++) {
			PL_LOG_ERROR("%d ", ((int32_t*)node->range)[i + 1]);
		}
		PL_LOG_ERROR(")\n");
		return -1;
	case SCHEMA_DATATYPE_INT8:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(int8_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(int8_t, "%" PRId8,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(int8_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(int8_t, "%" PRId8,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_UINT8:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(uint8_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(uint8_t, "%" PRIu8,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(uint8_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(uint8_t, "%" PRIu8,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_INT16:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(int16_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(int16_t, "%" PRId16,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(int16_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(int16_t, "%" PRId16,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_UINT16:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(uint16_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(uint16_t, "%" PRIu16,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(uint16_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(uint16_t, "%" PRIu16,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_INT32:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(int32_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(int32_t, "%" PRId32,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(int32_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(int32_t, "%" PRId32,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_UINT32:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(uint32_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(uint32_t, "%" PRIu32,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(uint32_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(uint32_t, "%" PRIu32,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_INT64:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(int64_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(int64_t, "%" PRId64,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(int64_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(int64_t, "%" PRId64,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_UINT64:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(uint64_t, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(uint64_t, "%" PRIu64,
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(uint64_t, val, node->range)) {
				VALIDATE_LOG_ERR_REF(uint64_t, "%" PRIu64,
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_FLOAT_1:
	case SCHEMA_DATATYPE_FLOAT_2:
	case SCHEMA_DATATYPE_FLOAT_3:
	case SCHEMA_DATATYPE_FLOAT_E1:
	case SCHEMA_DATATYPE_FLOAT_E18:
		switch (node->range_type) {
		case SCHEMA_NODE_RANGETYPE_FIXED:
			if (VALIDATE_RANGE_FIXED(float, val, node->range)) {
				VALIDATE_LOG_ERR_FIXED(float, "%f",
						val, node->range);
				return -1;
			}
			return 0;
		case SCHEMA_NODE_RANGETYPE_REFS:
			if (VALIDATE_RANGE_REF(float, val, node->range)) {
				VALIDATE_LOG_ERR_REF(float, "%f",
						val, node->range);
				return -1;
			}
			return 0;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_16:
		// TODO: Validate characters in string?
		if (strlen((char *)val) > 15) {
			PL_LOG_ERROR("Set validation error - strlen > 15");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_32:
		if (strlen((char *)val) > 31) {
			PL_LOG_ERROR("Set validation error - strlen > 31");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_64:
		if (strlen((char *)val) > 63) {
			PL_LOG_ERROR("Set validation error - strlen > 63");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_128:
		if (strlen((char *)val) > 127) {
			PL_LOG_ERROR("Set validation error - strlen > 127");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_256:
		if (strlen((char *)val) > 255) {
			PL_LOG_ERROR("Set validation error - strlen > 255");
			return -1;
		}
		return 0;
	}
	/* should not reach here */
	PL_LOG_ERROR("Control should not have reached here");
	return -1;
}

/*
 * return 0 on success; -1 on failure
 */
int
dm_set_param_value(struct dm *dm, const struct dm_schema_node *node,
		const void *val)
{
	int ret = -1;
	int ret2 = -1;
	size_t datatype_size = 0;
	bool is_str = false;

	PL_LOG_VERBOSE("Changing %s", node->label);

	if ((!node) || (!val)) {
		PL_LOG_ERROR("Bad input %p, %p", node, val);
		return 0;
	}

	if ((ret = dm_lock(&dm->mutex_id)) != 0) {
		PL_LOG_ERROR("Failed to lock DM");
		return -1;
	}

	if (dm_get_datatype_size(node->datatype, &datatype_size, &is_str) != 0) {
		PL_LOG_ERROR("Unknown datatype %d", node->datatype);
		ret = -1;
	} else {
		if (dm_validate_set_value(node, val) != 0) {
			PL_LOG_ERROR("Validation failed");
			ret = -1;
		} else {
			if (is_str) {
				if (strlcpy(node->val, val, datatype_size) >=
						datatype_size) {
					PL_LOG_ERROR("array_size %d is short",
							datatype_size);
				}
			} else {
				memcpy(node->val, val, datatype_size);
			}
			PL_LOG_DEBUG("Node %s changed", node->label);
			if (node->val_updated) {
				*node->val_updated = 1;
			}
			ret = 0;
		}
	}

	if ((ret2 = dm_unlock(&dm->mutex_id)) != 0) {
		PL_LOG_ERROR("Failed to unlock DM");
		ret = -1;  // XXX: SET might have been successful
	}
	return ret;
}

int
dm_set_param_value_x(struct dm *dm, enum dm_type dm_type,
		const char *xpath, void *value)
{
	const struct dm_schema_node *sn = dm_get_node(dm, dm_type, xpath);
	if (sn != NULL) {
		PL_LOG_INFO("XPATH: %s", xpath);
		return dm_set_param_value(dm, sn, value);
	} else {
		PL_LOG_INFO("XPATH %s in DM %d is not found", xpath, dm_type);
	}
	return -1;
}

int
dm_reset_param(struct dm *dm, const struct dm_schema_node *node)
{
	if (node->def == NULL) {
		PL_LOG_ERROR("Can't reset, no default value for %s %d",
				node->label, node->category);
		return -1;
	}
	return dm_set_param_value(dm, node, node->def);
}

int
dm_reset_tree(struct dm *dm, const struct dm_schema_node *node)
{
	int ret = 0;
	int i;
	switch (node->category) {
	case SCHEMA_NODE_TYPE_CONFIG:
	case SCHEMA_NODE_TYPE_INVENTORY:
	case SCHEMA_NODE_TYPE_ATTRIBUTE:
		// init with static defaults defined in schema
		ret += dm_reset_param(dm, node);
		break;
	case SCHEMA_NODE_TYPE_ROOT:
	case SCHEMA_NODE_TYPE_OBJECT:
		for (i = 0; i < node->num_children; i++) {
			ret += dm_reset_tree(dm,
				((struct dm_schema_node **)node->val)[i]);
			if (ret != 0) {
				break;
			}
		}
		break;
	}
	if (ret != 0) {
		ret = -1;
	}
	return ret;
}

int
dm_reset_config_tree(struct dm *dm, const struct dm_schema_node *node)
{
	int ret = 0;
	int i;
	switch (node->category) {
	case SCHEMA_NODE_TYPE_CONFIG:
		// init with static defaults defined in schema
		ret += dm_reset_param(dm, node);
		break;
	case SCHEMA_NODE_TYPE_ROOT:
	case SCHEMA_NODE_TYPE_OBJECT:
		for (i = 0; i < node->num_children; i++) {
			ret += dm_reset_config_tree(dm,
				((struct dm_schema_node **)node->val)[i]);
			if (ret != 0) {
				break;
			}
		}
		break;
	}
	if (ret != 0) {
		ret = -1;
	}
	return ret;
}

#define PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, type, format)\
do {\
	pr_funp(pr_handle, "\tCategory: %d\n", (node)->category);\
	pr_funp(pr_handle, "\tValue: " format "\n", *(type*)(node)->val);\
	switch ((node)->range_type)\
	{\
	case SCHEMA_NODE_RANGETYPE_FIXED:\
		pr_funp(pr_handle, "\tMin: " format "\n", ((type*)(node)->range)[0]);\
		pr_funp(pr_handle, "\tMax: " format "\n", ((type*)(node)->range)[1]);\
		break;\
	case SCHEMA_NODE_RANGETYPE_REFS:\
		pr_funp(pr_handle, "\tMin: " format "\n", *(((type**)(node)->range)[0]));\
		pr_funp(pr_handle, "\tMax: " format "\n", *(((type**)(node)->range)[1]));\
		break;\
	default:\
		pr_funp(pr_handle, "\tMin: -\n");\
		pr_funp(pr_handle, "\tMax: -\n");\
	}\
	if ((node)->def) {\
		pr_funp(pr_handle, "\tDefault: " format "\n", *((type*)(node)->def));\
	} else {\
		pr_funp(pr_handle, "\tDefault: -\n");\
	}\
} while (0);
/*
	switch ((node)->def_type)\
	{\
	case SCHEMA_NODE_DEFTYPE_FIXED:\
		pr_funp(pr_handle, "\tDefault: " format "\n", *((type*)(node)->def));\
		break;\
	case SCHEMA_NODE_DEFTYPE_REF:\
		pr_funp(pr_handle, "\tDefault: " format "\n", *(*((type**)(node)->def)));\
		break;\
	default:\
		pr_funp(pr_handle, "\tDefault: -\n");\
	}\
*/

int
dm_print_param(struct dm *dm, const struct dm_schema_node *node,
	dm_printf_funcp pr_funp, void *pr_handle)
{
	int ret = 0;
	int i = 0;

	if (!node)
		return 1;

	dm_lock(&dm->mutex_id);
	pr_funp(pr_handle, "Label: %s\n", node->label);
	switch (node->category) {
	case SCHEMA_NODE_TYPE_ROOT:
	case SCHEMA_NODE_TYPE_OBJECT:
		pr_funp(pr_handle, "\tNum of children: %u\n", node->num_children);
	}
	switch (node->datatype) {
	case SCHEMA_DATATYPE_BOOL:
		// TODO: There could be range with only true or false
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, uint8_t, "%" PRIu32);
		break;
	case SCHEMA_DATATYPE_ENUM:
		pr_funp(pr_handle, "\tValue: %d\n", *((int32_t*)node->val));
		if ((node->category == SCHEMA_NODE_TYPE_CONFIG) ||
				node->range) {
			//pr_funp(pr_handle, "EnumCnt: %d\n", ((int32_t*)node->range)[0]);
			pr_funp(pr_handle, "\tEnums:");
			for (i = 0; i < ((int32_t*)node->range)[0]; i++) {
				pr_funp(pr_handle, " %d,",
						((int32_t*)node->range)[i + 1]);
			}
			pr_funp(pr_handle, "\n");
		}
		if (node->def) {
			pr_funp(pr_handle, "\tDefault: %d\n", *((int32_t*)node->def));
		}
		break;
	case SCHEMA_DATATYPE_INT8:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, int8_t, "%" PRId8);
		break;
	case SCHEMA_DATATYPE_UINT8:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, uint8_t, "%" PRIu8);
		break;
	case SCHEMA_DATATYPE_INT16:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, int16_t, "%" PRId16);
		break;
	case SCHEMA_DATATYPE_UINT16:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, uint16_t, "%" PRIu16);
		break;
	case SCHEMA_DATATYPE_INT32:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, int32_t, "%" PRId32);
		break;
	case SCHEMA_DATATYPE_UINT32:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, uint32_t, "%" PRIu32);
		break;
	case SCHEMA_DATATYPE_INT64:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, uint64_t, "%" PRId64);
		break;
	case SCHEMA_DATATYPE_UINT64:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, uint64_t, "%" PRIu64);
		break;
	case SCHEMA_DATATYPE_FLOAT_1:
	case SCHEMA_DATATYPE_FLOAT_2:
	case SCHEMA_DATATYPE_FLOAT_3:
	case SCHEMA_DATATYPE_FLOAT_E1:
	case SCHEMA_DATATYPE_FLOAT_E18:
		PRINT_VAL_RANGE_DEF(pr_funp, pr_handle, node, float, "%f");
		break;
	case SCHEMA_DATATYPE_STRING_16:
		pr_funp(pr_handle, "\tCategory: %d\n", node->category);
		pr_funp(pr_handle, "\tValue: \"%s\"\n", node->val);
		pr_funp(pr_handle, "\tRange: 16\n");
		break;
	case SCHEMA_DATATYPE_STRING_32:
		pr_funp(pr_handle, "\tCategory: %d\n", node->category);
		pr_funp(pr_handle, "\tValue: \"%s\"\n", node->val);
		pr_funp(pr_handle, "\tRange: 32\n");
		break;
	case SCHEMA_DATATYPE_STRING_64:
		pr_funp(pr_handle, "\tCategory: %d\n", node->category);
		pr_funp(pr_handle, "\tValue: \"%s\"\n", node->val);
		pr_funp(pr_handle, "\tRange: 64\n");
		break;
	case SCHEMA_DATATYPE_STRING_128:
		pr_funp(pr_handle, "\tCategory: %d\n", node->category);
		pr_funp(pr_handle, "\tValue: \"%s\"\n", node->val);
		pr_funp(pr_handle, "\tRange: 128\n");
		break;
	case SCHEMA_DATATYPE_STRING_256:
		pr_funp(pr_handle, "\tCategory: %d\n", node->category);
		pr_funp(pr_handle, "\tValue: \"%s\"\n", node->val);
		pr_funp(pr_handle, "\tRange: 256\n");
		break;
	default:
		ret = 0;
	}
	pr_funp(pr_handle, "\n");
	dm_unlock(&dm->mutex_id);
	return ret;
}

int
dm_print_param_x(struct dm *dm, enum dm_type dm_type, const char *xpath,
	dm_printf_funcp pr_funp, void *pr_handle)
{
	const struct dm_schema_node *sn = dm_get_node(dm, dm_type, xpath);
	if (sn != NULL) {
		pr_funp(pr_handle, "XPATH: %s\n", xpath);
		return dm_print_param(dm, sn, pr_funp, pr_handle);
	} else {
		pr_funp(pr_handle, "XPATH %s in DM %d is not found\n",
				xpath, dm_type);
	}
	return -1;
}

int
dm_print_tree(struct dm *dm, const struct dm_schema_node *node,
	dm_printf_funcp pr_funp, void *pr_handle)
{
	int i;
	switch (node->category) {
	case SCHEMA_NODE_TYPE_ROOT:
	case SCHEMA_NODE_TYPE_OBJECT:
		for (i = 0; i < node->num_children; i++) {
			dm_print_tree(dm,
				((struct dm_schema_node **)node->val)[i],
				pr_funp, pr_handle);
		}
		break;
	default:
		dm_print_param(dm, node, pr_funp, pr_handle);
	}
	return 0;
}
