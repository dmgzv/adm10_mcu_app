#ifndef _OSPL_H_
#define _OSPL_H_

//#include <assert.h>
//#include <errno.h>
//#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <pthread.h>
//#include <arpa/inet.h>

#include "cmsis_os.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "endian.h"

#define pl_assert(...)

#define pl_atoi         atoi

#define pl_fopen        fopen
#define pl_fclose       fclose
#define pl_fscanf       fscanf
#define pl_fprintf      fprintf
#define pl_snprintf     snprintf
#define pl_fread        fread
#define pl_fwrite       fwrite
#define pl_rename       rename
#define pl_unlink       unlink

#define pl_read         read
#define pl_close        close

#define pl_recv         recv
#define pl_send         send
#define pl_setsockopt   setsockopt
#define pl_socket       socket
#define pl_bind         bind
#define pl_listen       listen
#define pl_accept       accept

#define pl_select       select

#define pl_fcntl        fcntl

#define pl_errno        errno
#define PL_EAGAIN       EAGAIN
#define PL_EWOULDBLOCK  EWOULDBLOCK
/*
typedef pthread_mutex_t dm_mutex_id_t;

#define pl_mutex_init   pthread_mutex_init
#define pl_mutex_lock   pthread_mutex_lock
#define pl_mutex_unlock pthread_mutex_unlock
*/

#define dm_mutex_id_t		osMutexId

#define pl_htobe16      htobe16
#define pl_htobe32      htobe32
#define pl_htobe64      htobe64

#define pl_be16toh      be16toh
#define pl_be32toh      be32toh
#define pl_be64toh      be64toh

struct pl_tftp_server {
	uint8_t  ipaddr[16];
	uint16_t port;
	uint8_t  path[128];
};

/*
 * Initialize listening socket.
 * Returns the listen socket if no error. On error returns -1
 */
int pl_init_listen_sock(in_addr_t addr, uint16_t port);

/* Returns 0 on success */
int pl_select_one(int listen_sock, int *mgr_sock, bool read_op, bool *ready);

int pl_upload_schema_file(struct pl_tftp_server *srv);
int pl_download_sw_image(struct pl_tftp_server *srv);
int pl_update_sw(struct pl_tftp_server *srv);
int pl_get_sw_update_status(struct pl_tftp_server *srv);

int pl_printf(void *pr_handle, const char *fmt, ...);

void pl_restart_process(uint16_t port);
#endif  /* _OSPL_H_ */
