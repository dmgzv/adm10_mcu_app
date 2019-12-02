#include "devapi.h"
#include "ospl.h"
#include "dev_agent_conf.h"
#include "dev_agent_log.h"
#include "dm_link.h"

static int
load_startup_config_f(const char *filename, void *cfg, size_t cfg_len)
{
	return 0;
}

int
agent_load_startup_config(void *cfg, size_t cfg_len)
{
	return 0;
}

int
load_startup_config()
{
	return 0;
}

static int
write_startup_config_f(const char *filename, const void *cfg, size_t cfg_len)
{
	return 0;
}

enum devapi_card_type
get_card_size()
{
	//TODO: get_card_size
	return DEVAPI_FULL_SLOT;
}

