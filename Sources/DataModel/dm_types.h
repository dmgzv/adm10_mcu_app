/* Auto-generated with cmd-line: c:\python27\scripts\pyang --plugindir .\plugin -V -f dm_codegen_plugin --dm-name adm10 YangFiles\ipg-common@2019-08-10.yang YangFiles\ipg-sfp@2019-11-27.yang YangFiles\ipg-adm-10@2019-11-28.yang YangFiles\ipg-hidden@2019-08-28.yang */
#ifndef _DM_TYPES_H_
#define _DM_TYPES_H_

#include <stdint.h>
#include <stddef.h>

#define SCHEMA_DATATYPE_NONE 0
#define SCHEMA_DATATYPE_BOOL 1
#define SCHEMA_DATATYPE_ENUM 2
#define SCHEMA_DATATYPE_INT8 3
#define SCHEMA_DATATYPE_UINT8 4
#define SCHEMA_DATATYPE_INT16 5
#define SCHEMA_DATATYPE_UINT16 6
#define SCHEMA_DATATYPE_INT32 7
#define SCHEMA_DATATYPE_UINT32 8
#define SCHEMA_DATATYPE_INT64 9
#define SCHEMA_DATATYPE_UINT64 10
#define SCHEMA_DATATYPE_FLOAT_1 11
#define SCHEMA_DATATYPE_FLOAT_2 12
#define SCHEMA_DATATYPE_FLOAT_3 13
#define SCHEMA_DATATYPE_FLOAT_E1 14
#define SCHEMA_DATATYPE_FLOAT_E18 15
#define SCHEMA_DATATYPE_STRING_16 16
#define SCHEMA_DATATYPE_STRING_32 17
#define SCHEMA_DATATYPE_STRING_64 18
#define SCHEMA_DATATYPE_STRING_128 19
#define SCHEMA_DATATYPE_STRING_256 20

#define SCHEMA_NODE_TYPE_ROOT 0
#define SCHEMA_NODE_TYPE_CONFIG 1
#define SCHEMA_NODE_TYPE_INVENTORY 2
#define SCHEMA_NODE_TYPE_MONITOR 3
#define SCHEMA_NODE_TYPE_FAULT 4
#define SCHEMA_NODE_TYPE_GAUGE 5
#define SCHEMA_NODE_TYPE_COUNTER 6
#define SCHEMA_NODE_TYPE_ATTRIBUTE 7
#define SCHEMA_NODE_TYPE_OBJECT 8

#define SCHEMA_NODE_RANGETYPE_NONE 0
#define SCHEMA_NODE_RANGETYPE_FIXED 1
#define SCHEMA_NODE_RANGETYPE_REFS 2
#define SCHEMA_NODE_RANGETYPE_ENUMS 3

typedef struct  schema_node {
  const char *label;
  const void *range;
  const void *def;
  void *val;
  uint8_t *val_updated;
  const uint16_t num_children;
  const uint8_t category;
  const uint8_t datatype;
  const uint8_t range_type;
//   const uint8_t def_type;
} schema_node_t;

#endif  /* _DM_TYPES_H_ */
