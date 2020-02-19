/* Auto-generated with cmd-line: c:\python27\scripts\pyang -p .\yang\ECS --plugindir .\plugin -V -f dm_codegen_plugin --dm-name adm_10_clbr yang\ADM-10\ipg-common@2019-08-10.yang yang\ADM-10\ipg-adm-10-clbr@2020-02-10.yang */
#include "dm_adm_10_clbr.h"

adm_10_clbr_config_values_t __attribute__((section ("CCM_DATA"))) adm_10_clbr_config_values;

const adm_10_clbr_config_fixed_defaults_t adm_10_clbr_config_fixed_defaults = {
  .adm_10_clbr_2020_02_10 = {  /* /adm-10-clbr-2020-02-10 */
    .serial_number = "0000-0000-0000",
  },  /* adm_10_clbr_2020_02_10 */
};  /* adm_10_clbr_config_fixed_defaults */

const schema_node_t sn_adm_10_clbr_2020_02_10__serial_number = {
  .label = "serial-number",
  .category = SCHEMA_NODE_TYPE_CONFIG,
  .datatype = SCHEMA_DATATYPE_STRING_64,
  .range_type = SCHEMA_NODE_RANGETYPE_NONE,
//   .def_type = SCHEMA_NODE_DEFTYPE_FIXED,
  .range = NULL,
  .def = &adm_10_clbr_config_fixed_defaults.adm_10_clbr_2020_02_10.serial_number,
  .val = &adm_10_clbr_config_values.adm_10_clbr_2020_02_10.serial_number,
  .val_updated = NULL,
  .num_children = 0,
};
const schema_node_t *sn_adm_10_clbr_2020_02_10_children[1] = {
  &sn_adm_10_clbr_2020_02_10__serial_number,
};
const schema_node_t sn_adm_10_clbr_2020_02_10 = {
  .label = "adm-10-clbr-2020-02-10",
  .category = SCHEMA_NODE_TYPE_OBJECT,
  .datatype = SCHEMA_DATATYPE_NONE,
  .range_type = SCHEMA_NODE_RANGETYPE_NONE,
//   .def_type = SCHEMA_NODE_DEFTYPE_NONE,
  .range = NULL,
  .def = NULL,
  .val = &sn_adm_10_clbr_2020_02_10_children[0],
  .val_updated = NULL,
  .num_children = 1,
};
const schema_node_t *sn_adm_10_clbr_root__children[1] = {
  &sn_adm_10_clbr_2020_02_10,
};
const schema_node_t sn_adm_10_clbr_root_ = {
  .label = "",
  .category = SCHEMA_NODE_TYPE_ROOT,
  .datatype = SCHEMA_DATATYPE_NONE,
  .range_type = SCHEMA_NODE_RANGETYPE_NONE,
//   .def_type = SCHEMA_NODE_DEFTYPE_NONE,
  .range = NULL,
  .def = NULL,
  .val = &sn_adm_10_clbr_root__children[0],
  .val_updated = NULL,
  .num_children = 1,
};
