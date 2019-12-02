#include <assert.h>

#include "dm.h"
#include "dm_adm10.h"
#include "dev_agent_log.h"

static const struct schema_node *
get_root_node(enum dm_type dm_type)
{
	switch (dm_type) {
	case DM_TYPE_MGMT:
		return &sn_adm10_root_;
	case DM_TYPE_CALIBRATION:
		return NULL;
	}
	return NULL;
}

static const struct schema_node *
get_node(enum dm_type dm_type, const char *xpath)
{
	assert(xpath != NULL);
	const struct schema_node *sn = get_root_node(dm_type);
	return get_schema_node(sn, xpath);
}

static void
print_param(enum dm_type dm_type, const char *xpath)
{
	const struct schema_node *sn = get_node(DM_TYPE_MGMT, xpath);
	if (sn != NULL) {
		AGT_LOG_INFO("XPATH: %s", xpath);
		dm_print_param(sn);
	} else {
		AGT_LOG_INFO("XPATH %s in DM %d is not found", xpath, dm_type);
	}
}

static void
set_param_value(enum dm_type dm_type, const char *xpath, void *value)
{
	const struct schema_node *sn = get_node(DM_TYPE_MGMT, xpath);
	if (sn != NULL) {
		AGT_LOG_INFO("XPATH: %s", xpath);
		dm_set_param_value(sn, value);
	} else {
		AGT_LOG_INFO("XPATH %s in DM %d is not found", xpath, dm_type);
	}
}

int
main()
{
	int32_t i32;
	const struct schema_node *sn;
	dm_lock_init();
	dm_print_tree(&sn_adm10_root_);
	dm_reset_tree(&sn_adm10_root_);
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/__HIDDEN__/admin_state");
	i32 = 3;
	set_param_value(DM_TYPE_MGMT, "/__HIDDEN__/admin_state", &i32);
	print_param(DM_TYPE_MGMT, "/__HIDDEN__/admin_state");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/__HIDDEN__/service_state");
	i32 = 2;
	set_param_value(DM_TYPE_MGMT, "/__HIDDEN__/service_state", &i32);
	print_param(DM_TYPE_MGMT, "/__HIDDEN__/service_state");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/adm10_2019_08_01/adm/line[1]/sfp_plus/freq_spacing");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "//");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/adm10_2019_08_01/adm/line[1]/sfp_plus/freq_spacing/");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/adm10_2019_08_01//adm/line[1]/sfp_plus/freq_spacing/");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "//adm10_2019_08_01/adm/line[1]/sfp_plus/freq_spacing");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "adm10_2019_08_01/adm/line[1]/sfp_plus/freq_spacing");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/adm10_2019_08_01/adm/line[1]/sfp_plus/freq_spacing/fail");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/adm10_2019_08_01/adm/line[5]/sfp_plus/freq_spacing/fail");
	AGT_LOG_DEBUG("----------------------------------------");
	print_param(DM_TYPE_MGMT, "/adm10_2019_08_01/adm/line[0]/sfp_plus/freq_spacing/fail");
	AGT_LOG_DEBUG("----------------------------------------");
}
