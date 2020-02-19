/* Auto-generated with cmd-line: c:\python27\scripts\pyang -p .\yang\ECS --plugindir .\plugin -V -f dm_codegen_plugin --dm-name adm_10_clbr yang\ADM-10\ipg-common@2019-08-10.yang yang\ADM-10\ipg-adm-10-clbr@2020-02-10.yang */
#ifndef _DM_ADM_10_CLBR_H_
#define _DM_ADM_10_CLBR_H_

#include "dm_types.h"

extern const schema_node_t sn_adm_10_clbr_2020_02_10__serial_number;
extern const schema_node_t sn_adm_10_clbr_root_;

/*******************************************************************************
 * OBJECT LIST ELEMENTS COUNT                                                  *
*******************************************************************************/

/*******************************************************************************
 * ENUMERATION #DEFINES                                                        *
*******************************************************************************/
typedef struct  adm_10_clbr_config_values {
  struct  {  /* /adm-10-clbr-2020-02-10 */
    char    serial_number[65];
  } adm_10_clbr_2020_02_10;
} adm_10_clbr_config_values_t;

typedef struct  adm_10_clbr_config_fixed_defaults {
  struct  {  /* /adm-10-clbr-2020-02-10 */
    char    serial_number[65];
  } adm_10_clbr_2020_02_10;
} adm_10_clbr_config_fixed_defaults_t;

#endif  /* _DM_ADM_10_CLBR_H_ */
