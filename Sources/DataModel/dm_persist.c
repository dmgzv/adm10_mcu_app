#include "dm.h"
#include "devapi.h"
#include "pl.h"
#include "dev_agent_conf.h"

int
dm_load_startup_config(const char *dm_schema_id, enum dm_type dm_type,
		void *mgmt_cfg, size_t mgmt_cfg_len,
		void *clbr_cfg, size_t clbr_cfg_len)
{
	/*
	int ret;

	// XXX: dm_type is unused.
	
	ret = dm_load_startup_config_f(STARTUP_CONFIG_FILE, dm_schema_id,
			mgmt_cfg, mgmt_cfg_len, clbr_cfg, clbr_cfg_len);
	if (ret == -1) {
		PL_LOG_INFO("Failed to load from %s", STARTUP_CONFIG_FILE);
		ret = dm_load_startup_config_f(BACKUP_CONFIG_FILE, dm_schema_id,
				mgmt_cfg, mgmt_cfg_len, clbr_cfg, clbr_cfg_len);
		if (ret == -1) {
			PL_LOG_INFO("Failed to load from %s",
					BACKUP_CONFIG_FILE);
		} else {
			PL_LOG_INFO("Loaded from %s", BACKUP_CONFIG_FILE);
		}
	} else {
		PL_LOG_INFO("Loaded from %s", STARTUP_CONFIG_FILE);
	}
	return ret;
	*/
	return 0;
}

int
dm_write_startup_config(const char *dm_schema_id, enum dm_type dm_type,
		const void *mgmt_cfg, size_t mgmt_cfg_len,
		const void *clbr_cfg, size_t clbr_cfg_len)
{
	/*
	FILE *fp;
	int ret;

	// XXX: dm_type is unused.

	// rename startup config file as backup file
	if ((pl_rename(STARTUP_CONFIG_FILE, BACKUP_CONFIG_FILE) == -1) &&
			(errno != ENOENT)) {
		PL_LOG_ERROR("Failed to rename %s to %s. errno=%d",
				STARTUP_CONFIG_FILE, BACKUP_CONFIG_FILE, errno);
	}
	
	// create and write to startup config file
	fp = pl_fopen(STARTUP_CONFIG_FILE, "wb");
	if (fp == NULL) {
		PL_LOG_INFO("Failed to open %s. errno=%d", STARTUP_CONFIG_FILE,
				errno);
		return -1;
	}
	// write format and schema version
	ret = pl_fprintf(fp, "%d %s\r\n", DATA_FORMAT_ID, dm_schema_id);
	if (ret < 0) {
		PL_LOG_ERROR("Failed to write header to file, ret=%d", ret);
		goto WRITE_FAILURE;
	}
	if (mgmt_cfg_len > 0) {
		ret = pl_fwrite(mgmt_cfg, mgmt_cfg_len, 1, fp);
		if (ret != 1) {
			PL_LOG_ERROR("Failed to write config file, ret=%d", ret);
			goto WRITE_FAILURE;
		}
	}
	if (clbr_cfg_len > 0) {
		ret = pl_fwrite(clbr_cfg, clbr_cfg_len, 1, fp);
		if (ret != 1) {
			PL_LOG_ERROR("Failed to write config file, ret=%d", ret);
			goto WRITE_FAILURE;
		}
	}
	pl_fclose(fp);
	return 0;

WRITE_FAILURE:
	pl_fclose(fp);
	if ((pl_unlink(STARTUP_CONFIG_FILE) == -1) && (errno != ENOENT)) {
		PL_LOG_ERROR("Failed to delete %s. errno=%d",
				STARTUP_CONFIG_FILE, errno);
	}
	return -1;
	*/
	return 0;
}
