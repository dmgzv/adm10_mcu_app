#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "dm.h"
#include "dm_link.h"
#include "dev_agent_log.h"

const struct schema_node *
get_child_schema_node(const struct schema_node *parent,
		const char *label, size_t label_len)
{
	int i;

	//assert(parent != NULL);
	//assert(label != NULL);

	if (parent->category == SCHEMA_NODE_TYPE_ROOT ||
			parent->category == SCHEMA_NODE_TYPE_OBJECT) {
		for (i = 0; i < parent->num_children; i++) {
			const struct schema_node *ch =
				((struct schema_node **)parent->val)[i];
			if (strncmp(ch->label, label, label_len) == 0) {
				if (ch->label[label_len] != '\0') {
					LOG_DEBUG("Partial match in %s len:%d",
							ch->label, label_len);
					continue;
				}
				return ch;
			}
		}
	}
	return NULL;
}

const struct schema_node *
get_schema_node(const struct schema_node *root, const char *xpath)
{
	const struct schema_node *sn = root;
	const char *sep_p = NULL;
	const char *xp2 = xpath;

	//assert(root != NULL);
	//assert(xpath != NULL);
	sep_p = strchr(xp2, '/');
	if (sep_p != xp2) {
		LOG_DEBUG("Bad xpath %s", xpath);
		return NULL;
	}
	while (sep_p != NULL) {
		xp2 = sep_p + 1;
		sep_p = strchr(xp2, '/');
		if (sn->category == SCHEMA_NODE_TYPE_ROOT ||
				sn->category == SCHEMA_NODE_TYPE_OBJECT) {
			// LOG_VERBOSE("OBJECT %s in %s", label, xpath);
			sn = get_child_schema_node(sn, xp2,
					(sep_p ? (sep_p - xp2) : strlen(xp2)));
			if (sn == NULL) {
				LOG_DEBUG("'%s' not in XPATH %s",
						(xp2-1), xpath);
				return NULL;
			} else if (sep_p == NULL) {
				return sn;
			}
		} else {
			LOG_DEBUG("'%s' not in XPATH %s", (xp2-1), xpath);
			return NULL;
		}
	}
	return NULL;
}

const struct schema_node *
dm_get_node(enum dm_type dm_type, const char *xpath)
{
	//assert(xpath != NULL);
	const struct schema_node *sn = get_root_node(dm_type);
	return get_schema_node(sn, xpath);
}

/*
 * returns 0 on success; -1 on error
 */
int
get_datatype_size(uint8_t datatype, size_t *size, bool *is_str)
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
		LOG_ERROR("Unknown datatype %d", datatype);
		return -1;
	}
	return 0;
}

/*
 * return 0 on success; -1 on failure
 */
int
dm_get_param_value(const struct schema_node *node, void *val)
{
	int ret = -1;
	size_t datatype_size = 0;
	bool is_str = false;

	if ((!node) || (!val))
		return 0;

	if ((ret = dm_lock()) != 0) {
		LOG_ERROR("Failed to lock DM");
		return -1;
	}

	if (get_datatype_size(node->datatype, &datatype_size, &is_str) != 0) {
		LOG_ERROR("Unknown datatype %d", node->datatype);
		ret = -1;
	} else {
		if (is_str) {
			if (strlcpy(val, node->val, datatype_size) >=
					datatype_size) {
				LOG_ERROR("array_size %d is short",
						datatype_size);
			}
		} else {
			memcpy(val, node->val, datatype_size);
		}
		ret = 0;
	}

	if ((ret = dm_unlock()) != 0) {
		LOG_ERROR("Failed to unlock DM");
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
	LOG_ERROR("Set validation error- " fmt " not in [" fmt " .. " fmt "]",\
			*(type*)(val_p), ((type*)(range_p))[0], \
			((type*)(range_p))[1])

#define VALIDATE_LOG_ERR_REF(type, fmt, val_p, range_ref_p) \
	LOG_ERROR("Set validation error - " fmt " not in [" fmt " .. " fmt "]",\
			*(type*)(val_p), *((type**)(range_ref_p))[0], \
			*((type**)(range_ref_p))[1])

/*
 * NOTE: This function does not lock DM.
 * return 0 on success; -1 on failure
 */
static int
dm_validate_set_value(const struct schema_node *node, const void *val)
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
			LOG_ERROR("%d is not bool value", *(uint8_t*)val);
			return -1;
		}
		return -1;
	case SCHEMA_DATATYPE_ENUM:
		for (i = 0; i < ((int32_t*)node->range)[0]; i++) {
			if (*((int32_t*)val) == ((int32_t*)node->range)[i+1]) {
				return 0;
			}
		}
		LOG_ERROR("Enum %d is not in set ( ", *((int32_t*)val));
		for (i = 0; i < ((int32_t*)node->range)[0]; i++) {
			LOG_ERROR("%d ", ((int32_t*)node->range)[i + 1]);
		}
		LOG_ERROR(")\n");
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
			LOG_ERROR("Set validation error - strlen > 15");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_32:
		if (strlen((char *)val) > 31) {
			LOG_ERROR("Set validation error - strlen > 31");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_64:
		if (strlen((char *)val) > 63) {
			LOG_ERROR("Set validation error - strlen > 63");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_128:
		if (strlen((char *)val) > 127) {
			LOG_ERROR("Set validation error - strlen > 127");
			return -1;
		}
		return 0;
	case SCHEMA_DATATYPE_STRING_256:
		if (strlen((char *)val) > 255) {
			LOG_ERROR("Set validation error - strlen > 255");
			return -1;
		}
		return 0;
	}
	/* should not reach here */
	LOG_ERROR("Control should not have reached here");
	return -1;
}

/*
 * return 0 on success; -1 on failure
 */
int
dm_set_param_value(const struct schema_node *node, const void *val)
{
	int ret = -1;
	size_t datatype_size = 0;
	bool is_str = false;

	LOG_VERBOSE("Changing %s", node->label);

	if ((!node) || (!val)) {
		LOG_ERROR("Bad input %p, %p", node, val);
		return 0;
	}

	if ((ret = dm_lock()) != 0) {
		LOG_ERROR("Failed to lock DM");
		return -1;
	}

	if (get_datatype_size(node->datatype, &datatype_size, &is_str) != 0) {
		LOG_ERROR("Unknown datatype %d", node->datatype);
		ret = -1;
	} else {
		if (dm_validate_set_value(node, val) != 0) {
			LOG_ERROR("Validation failed");
			ret = -1;
		}
		if (is_str) {
			if (strlcpy(node->val, val, datatype_size) >=
					datatype_size) {
				LOG_ERROR("array_size %d is short",
						datatype_size);
			}
		} else {
			memcpy(node->val, val, datatype_size);
		}
		LOG_DEBUG("Node %s changed", node->label);
		if (node->val_updated) {
			*node->val_updated = 1;
		}
		ret = 0;
	}

	if ((ret = dm_unlock()) != 0) {
		LOG_ERROR("Failed to unlock DM");
		ret = -1;  // TODO: SET might have been successful
	}
	return ret;
}

int
dm_set_param_value_x(enum dm_type dm_type, const char *xpath, void *value)
{
	const struct schema_node *sn = dm_get_node(dm_type, xpath);
	if (sn != NULL) {
		LOG_INFO("XPATH: %s", xpath);
		return dm_set_param_value(sn, value);
	} else {
		LOG_INFO("XPATH %s in DM %d is not found", xpath, dm_type);
	}
	return -1;
}

int
dm_reset_param(const struct schema_node *node)
{
	if (node->def == NULL) {
		LOG_ERROR("Can't reset, no default value for %s %d",
				node->label, node->category);
		return -1;
	}
	return dm_set_param_value(node, node->def);
}

int
dm_reset_tree(const struct schema_node *node)
{
	int ret = 0;
	int i;
	switch (node->category) {
	case SCHEMA_NODE_TYPE_CONFIG:
	case SCHEMA_NODE_TYPE_INVENTORY:
	case SCHEMA_NODE_TYPE_ATTRIBUTE:
		// init with static defaults defined in schema
		ret += dm_reset_param(node);
		break;
	case SCHEMA_NODE_TYPE_ROOT:
	case SCHEMA_NODE_TYPE_OBJECT:
		for (i = 0; i < node->num_children; i++) {
			ret += dm_reset_tree(
				((struct schema_node **)node->val)[i]);
		}
		break;
	}
	if (ret != 0) {
		ret = -1;
	}
	return ret;
}

#define PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs, node, type, format)\
do {\
	dm_printf(priv, "%sCategory: %d\r", DM_PRINT_TABS(tabs + 1), (node)->category);\
	dm_printf(priv, "%sValue: " format "\r", DM_PRINT_TABS(tabs + 1), *(type*)(node)->val);\
	switch ((node)->range_type)\
	{\
	case SCHEMA_NODE_RANGETYPE_FIXED:\
		dm_printf(priv, "%sMin: " format "\r", DM_PRINT_TABS(tabs + 1), ((type*)(node)->range)[0]);\
		dm_printf(priv, "%sMax: " format "\r", DM_PRINT_TABS(tabs + 1), ((type*)(node)->range)[1]);\
		break;\
	case SCHEMA_NODE_RANGETYPE_REFS:\
		dm_printf(priv, "%sMin: " format "\r", DM_PRINT_TABS(tabs + 1), *(((type**)(node)->range)[0]));\
		dm_printf(priv, "%sMax: " format "\r", DM_PRINT_TABS(tabs + 1), *(((type**)(node)->range)[1]));\
		break;\
	default:\
		dm_printf(priv, "%sMin: -\r", DM_PRINT_TABS(tabs + 1));\
		dm_printf(priv, "%sMax: -\r", DM_PRINT_TABS(tabs + 1));\
	}\
	if ((node)->def) {\
		dm_printf(priv, "%sDefault: " format "\r", DM_PRINT_TABS(tabs + 1), *((type*)(node)->def));\
	} else {\
		dm_printf(priv, "%sDefault: -\r", DM_PRINT_TABS(tabs + 1));\
	}\
} while (0);
/*
	switch ((node)->def_type)\
	{\
	case SCHEMA_NODE_DEFTYPE_FIXED:\
		dm_printf(priv, "\tDefault: " format, *((type*)(node)->def));\
		break;\
	case SCHEMA_NODE_DEFTYPE_REF:\
		dm_printf(priv, "\tDefault: " format, *(*((type**)(node)->def)));\
		break;\
	default:\
		dm_printf(priv, "\tDefault: -");\
	}\
*/

#define DM_PRINT_TABS(tabs_cnt) \
((tabs_cnt) == 0 ? "" : \
((tabs_cnt) == 1 ? "\t" : \
((tabs_cnt) == 2 ? "\t\t" : \
((tabs_cnt) == 3 ? "\t\t\t" : \
((tabs_cnt) == 4 ? "\t\t\t\t" : \
((tabs_cnt) == 5 ? "\t\t\t\t\t" : \
((tabs_cnt) == 6 ? "\t\t\t\t\t\t" : \
((tabs_cnt) == 7 ? "\t\t\t\t\t\t\t" : "\t\t\t\t\t\t\t\t"))))))))

int __dm_print_param(const struct schema_node *node, int (*dm_printf)(void* priv, char *str, ...), void *priv, int tabs)
{
	int ret = 0;
	if (!node)
		return 1;
	dm_lock();
	dm_printf(priv, "%s%s {\r", DM_PRINT_TABS(tabs), node->label);
	switch (node->datatype)
	{
	case SCHEMA_DATATYPE_BOOL:
		//TODO: There could be range with only true or false
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs, node, uint8_t, "%u");
		break;
	case SCHEMA_DATATYPE_ENUM:
		dm_printf(priv, "%sCategory: %d\r", DM_PRINT_TABS(tabs + 1), (node)->category);
		dm_printf(priv, "%sValue: %d\r", DM_PRINT_TABS(tabs + 1), *((int32_t*)node->val));
		// TODO: For CONFIG param which is enum, range is must
		if ((node->category == SCHEMA_NODE_TYPE_CONFIG) || node->range) {
			//dm_printf(priv, "Enums count: %d", ((int32_t*)node->range)[0]);
			dm_printf(priv, "%sEnums:", DM_PRINT_TABS(tabs + 1));
			for (int i = 0; i < ((int32_t*)node->range)[0]; i++)
			{
				dm_printf(priv, " %d,", ((int32_t*)node->range)[i + 1]);
			}
			dm_printf(priv, "\r");
		}
		if (node->def) {
			dm_printf(priv, "%sDefault: %d\r", DM_PRINT_TABS(tabs + 1), *((int32_t*)node->def));
		}
		break;
	case SCHEMA_DATATYPE_INT8:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, int8_t, "%d");
		break;
	case SCHEMA_DATATYPE_UINT8:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, uint8_t, "%u");
		break;
	case SCHEMA_DATATYPE_INT16:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, int16_t, "%d");
		break;
	case SCHEMA_DATATYPE_UINT16:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, uint16_t, "%u");
		break;
	case SCHEMA_DATATYPE_INT32:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, int32_t, "%d");
		break;
	case SCHEMA_DATATYPE_UINT32:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, uint32_t, "%u");
		break;
	case SCHEMA_DATATYPE_INT64:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, uint64_t, "%lld");
		break;
	case SCHEMA_DATATYPE_UINT64:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, uint64_t, "%llu");
		break;
	case SCHEMA_DATATYPE_FLOAT_1:
	case SCHEMA_DATATYPE_FLOAT_2:
	case SCHEMA_DATATYPE_FLOAT_3:
	case SCHEMA_DATATYPE_FLOAT_E1:
	case SCHEMA_DATATYPE_FLOAT_E18:
		PRINT_VAL_RANGE_DEF(dm_printf, priv, tabs + 1, node, float, "%f");
		break;
	case SCHEMA_DATATYPE_STRING_16:
	case SCHEMA_DATATYPE_STRING_32:
	case SCHEMA_DATATYPE_STRING_64:
	case SCHEMA_DATATYPE_STRING_128:
	case SCHEMA_DATATYPE_STRING_256:
		dm_printf(priv, "%sCategory: %d\r", DM_PRINT_TABS(tabs + 1), (node)->category);
		dm_printf(priv, "%sValue: %s\r", DM_PRINT_TABS(tabs + 1), ((char*)node->val));
	default:
		ret = 0;
	}
	dm_printf(priv, "%s}\r", DM_PRINT_TABS(tabs));
	dm_unlock();
	return ret;
}

int __dm_print_param_x(enum dm_type dm_type, const char *xpath, int (*dm_printf)(void* priv, char *str, ...), void *priv, int tabs)
{
	const struct schema_node *sn = dm_get_node(dm_type, xpath);
	if (sn != NULL) {
		return __dm_print_param(sn, dm_printf, priv, tabs + 1);
	}
	return -1;
}

int __dm_print_tree(const struct schema_node *node, int (*dm_printf)(void* priv, char *str, ...), void *priv, int tabs)
{
	int i;
	switch (node->category) {
	case SCHEMA_NODE_TYPE_ROOT:
	case SCHEMA_NODE_TYPE_OBJECT:
		dm_printf(priv, "%s%s {\r", DM_PRINT_TABS(tabs), node->label);
		for (i = 0; i < node->num_children; i++) {
			__dm_print_tree(((struct schema_node **)node->val)[i] , dm_printf, priv, tabs + 1);
		}
		dm_printf(priv, "%s}\r", DM_PRINT_TABS(tabs));
		break;
	default:
		__dm_print_param(node, dm_printf, priv, tabs);
	}
	return 0;
}


