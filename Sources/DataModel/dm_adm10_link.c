#include <string.h>

#include "dm.h"
#include "dm_adm10.h"
#include "dm_link.h"

const char *dm_schema_id = "adm-10-2019-11-28";

extern struct adm10_attribute_values adm10_attribute_values;
extern struct adm10_config_values adm10_config_values;
extern struct adm10_fault_values adm10_fault_values;
extern struct adm10_inventory_values adm10_inventory_values;
extern struct adm10_monitor_values adm10_monitor_values;
extern struct adm10_gauge_values adm10_gauge_values;
extern struct adm10_counter_values adm10_counter_values;

#define DM_ATTRIBUTE_VAL adm10_attribute_values
#define DM_CONFIG_VAL adm10_config_values
#define DM_INVENTORY_VAL adm10_inventory_values
#define DM_MONITOR_VAL adm10_monitor_values
#define DM_FAULT_VAL adm10_fault_values
//#define DM_COUNTER_VAL adm10_counter_values
#define DM_GAUGE_VAL adm10_gauge_values

#define SN_MGMT_ROOT sn_adm10_root_

int
dm_init_data_model()
{
	memset(&DM_ATTRIBUTE_VAL, 0, sizeof(DM_ATTRIBUTE_VAL));
	memset(&DM_CONFIG_VAL, 0, sizeof(DM_CONFIG_VAL));
	memset(&DM_INVENTORY_VAL, 0, sizeof(DM_INVENTORY_VAL));
	memset(&DM_MONITOR_VAL, 0, sizeof(DM_MONITOR_VAL));
	memset(&DM_FAULT_VAL, 0, sizeof(DM_FAULT_VAL));
	//memset(&DM_COUNTER_VAL, 0, sizeof(DM_COUNTER_VAL));
	memset(&DM_GAUGE_VAL, 0, sizeof(DM_GAUGE_VAL));
	
	dm_reset_tree(&SN_MGMT_ROOT);

	return 0;
}

void
dm_get_config_model(void **dm, size_t *size)
{
	*dm = &DM_CONFIG_VAL;
	*size = sizeof(DM_CONFIG_VAL);
}

const struct schema_node *
get_root_node(enum dm_type dm_type)
{
	switch (dm_type) {
	case DM_TYPE_MGMT:
		return &SN_MGMT_ROOT;
	case DM_TYPE_CALIBRATION:
		return NULL;
	}
	return NULL;
}

