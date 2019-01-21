#ifndef LWIP_PING_H
#define LWIP_PING_H

#include "lwip/ip_addr.h"

/**
 * PING_USE_SOCKETS: Set to 1 to use sockets, otherwise the raw api is used
 */
#ifndef PING_USE_SOCKETS
#define PING_USE_SOCKETS    LWIP_SOCKET
#endif

void ping_init(void);

#if !PING_USE_SOCKETS
void ping_start(void);
void ping_stop(void);
ip_addr_t ping_get_ip(void);
void ping_set_ip(ip_addr_t ping_addr);
uint32_t ping_get_state(void);
#endif /* !PING_USE_SOCKETS */

#endif /* LWIP_PING_H */
