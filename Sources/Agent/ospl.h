#ifndef _OSPL_H_
#define _OSPL_H_

//#include <stdio.h>
//#include <errno.h>
#include <stdint.h>
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include <stdbool.h>
//#include <pthread.h>

#define os_fopen        fopen
#define os_fclose       fclose
#define os_fscanf       fscanf
#define os_fprintf      fprintf
#define os_fread        fread
#define os_fwrite       fwrite
#define os_rename       rename
#define os_unlink       unlink

#define os_read         read
#define os_close        close

#define os_recv         recv
#define os_send         send

#define os_mutex_init   pthread_mutex_init
#define os_mutex_lock   pthread_mutex_lock
#define os_mutex_unlock pthread_mutex_unlock

#define STARTUP_CONFIG_FILE "/dev_agent/startup.cfg" // TODO: finalize the path
#define BACKUP_CONFIG_FILE "/dev_agent/startup.bak" // TODO: finalize the path

struct tftp_server {
	uint8_t  ipaddr[16];
	uint16_t port;
	uint8_t  path[128];
};

/*
 * Initialize listening socket.
 * Returns the listen socket if no error. On error returns -1
 */
int os_init_listen_sock(in_addr_t addr, uint16_t port);

/* Returns 0 on success */
int select_one(int listen_sock, int *mgr_sock, bool read, bool *ready);

int upload_schema_file(struct tftp_server *srv);

#endif  /* _OSPL_H_ */
