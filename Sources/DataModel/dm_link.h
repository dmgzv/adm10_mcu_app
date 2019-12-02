#ifndef _DM_LINK_H_
#define _DM_LINK_H_

#include "dm.h"
#include "dm_types.h"

int dm_init_data_model(void);
void dm_get_config_model(void **dm, size_t *size);
const struct schema_node *get_root_node(enum dm_type dm_type);

#endif  /* _DM_LINK_H_ */
