#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <libgen.h>
#define CLI  // TODO: comment out
#ifdef CLI
#include <stdio.h>
#include "dm.h"		// TODO: comment out
#endif  // CLI
#include "pl.h"


#define LISTEN_BACKLOG 1


int
dm_lock_init(dm_mutex_id_t *mutex)
{
	int ret;
	ret = pl_mutex_init(mutex, NULL);
	if (ret != 0) {
		PL_LOG_ERROR("Failed to init DM lock, errno=%d", ret);
		return -1;
	}
	PL_LOG_DEBUG("DM lock initialized");
	return 0;
}

int
dm_lock(dm_mutex_id_t *mutex)
{
	int ret;
	ret = pl_mutex_lock(mutex);
	if (ret != 0) {
		PL_LOG_ERROR("Failed to aquire DM lock, errno=%d", ret);
		return -1;
	}
	PL_LOG_DEBUG("Aquired DM lock");
	return 0;
}

int dm_unlock(dm_mutex_id_t *mutex)
{
	int ret;
	ret = pl_mutex_unlock(mutex);
	if (ret != 0) {
		PL_LOG_ERROR("Failed to release DM lock, errno=%d", ret);
		return -1;
	}
	PL_LOG_DEBUG("Released DM lock");
	return 0;
}

/* set/unset filedescriptor to non-blocking */
static int
set_nonblock(int fd)
{
	int val;

	val = pl_fcntl(fd, F_GETFL);
	if (val == -1) {
		PL_LOG_ERROR("fcntl(%d, F_GETFL) failed, errno=%d", fd, errno);
		return -1;
	}
	if (val & O_NONBLOCK) {
		return 0;
	}
	val |= O_NONBLOCK;
	if (pl_fcntl(fd, F_SETFL, val) == -1) {
		PL_LOG_ERROR("Failed to set %d to O_NONBLOCK, errno=%d",
				fd, errno);
		return -1;
	}
	return 0;
}

/* Allow local port reuse in TIME_WAIT */
static int
set_reuseaddr(int fd)
{
	int on = 1;

	if (pl_setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		PL_LOG_ERROR("setsockopt SO_REUSEADDR fd %d: %d", fd, errno);
		return -1;
	}
	return 0;
}

/*
 * Initialize listening socket.
 * Returns the listen socket if no error. On error returns -1
 */
int
pl_init_listen_sock(in_addr_t addr, uint16_t port)
{
	int listen_sock = -1;
	struct sockaddr_in agt_addr;

	if ((listen_sock = pl_socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		PL_LOG_ERROR("Failed to open socket, errno=%d.", errno);
		return -1;
	}
	PL_LOG_DEBUG("listen_sock=%d", listen_sock);

	if (set_nonblock(listen_sock)) {
		pl_close(listen_sock);
		return -1;
	}
	if (pl_fcntl(listen_sock, F_SETFD, FD_CLOEXEC) == -1) {
		PL_LOG_ERROR("socket: CLOEXEC: %d", errno);
		pl_close(listen_sock);
		return -1;
	}
	/* Socket options */
	if (set_reuseaddr(listen_sock) == -1)
		return -1;

	memset(&agt_addr, 0, sizeof(agt_addr));

	agt_addr.sin_family = AF_INET;
	agt_addr.sin_addr.s_addr = htonl(addr);
	agt_addr.sin_port = htons(port);

	if (pl_bind(listen_sock, (struct sockaddr *)&agt_addr,
				sizeof(agt_addr)) == -1) {
		PL_LOG_ERROR("Failed to bind agent port %d, errno=%d.",
				port, errno)
		return -1;
	}

	if (pl_listen(listen_sock, LISTEN_BACKLOG) == -1) {
		PL_LOG_ERROR("Failed to listen, errno=%d.", errno)
		return -1;
	}

	return listen_sock;
}

#ifdef CLI
extern struct dm g_dm;
void
process_cli(int fd)
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	while (1) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		select(fd+1, &fds, NULL, NULL, &tv);
		if (FD_ISSET(fd, &fds)) {
			char in_line[1024];
			if (fgets(in_line, 1024, stdin) == NULL) {
				return;
			}
			char *cmd = strtok(in_line, " ");
			if (cmd == NULL)
				return;
			char *xpath = strtok(NULL, " \n");
			if (xpath == NULL)
				return;
			if (!strcmp(cmd, "get")) {
				PL_LOG_INFO("%s %s", cmd, xpath);
				dm_print_param_x(&g_dm, DM_TYPE_MGMT, xpath, pl_printf, stdout);
			} else if (!strcmp(cmd, "set")) {
				char *type = strtok(NULL, " \n");
				if (type == NULL)
					return;
				char *val = strtok(NULL, " \n");
				if (val == NULL)
					return;
				PL_LOG_INFO("%s %s %s %s", cmd, xpath, type, val);
				if (!strcmp(type, "i32")) {
					int32_t i = atoi(val);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "u32")) {
					uint32_t i = atoi(val);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "i16")) {
					int16_t i = atoi(val);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "u16")) {
					uint16_t i = atoi(val);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "i8")) {
					int8_t i = atoi(val);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "u8")) {
					uint8_t i = atoi(val);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "i64")) {
					int64_t i = strtoll(val, NULL, 0);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "u64")) {
					uint64_t i = strtoll(val, NULL, 0);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "f")) {
					float i = strtof(val, NULL);
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, &i);
				} else if (!strcmp(type, "s")) {
					dm_set_param_value_x(&g_dm, DM_TYPE_MGMT, xpath, val);
				} else {
				}
			} else {
			}
		} else {
			break;
		}
	}
}
#endif  // CLI

int
pl_select_one(int listen_sock, int *mgr_sock, bool read_op, bool *ready)
{
	pl_assert(listen_sock >= 0);
	pl_assert(mgr_sock != NULL);
	pl_assert(ready != NULL);

	fd_set select_fds;
	int max_fd = FD_SETSIZE;
	int new_sock = -1;

	*ready = false;

	PL_LOG_VERBOSE("listen_sock=%d, mgr_sock=%d", listen_sock, *mgr_sock);

	FD_ZERO(&select_fds);
	FD_SET(listen_sock, &select_fds);
#ifdef CLI
	FD_SET(0, &select_fds);
#endif  // CLI
	if (*mgr_sock != -1)
		FD_SET(*mgr_sock, &select_fds);
	max_fd = (listen_sock > *mgr_sock) ? listen_sock : *mgr_sock;
	// PL_LOG_VERBOSE("max_fd=%d", max_fd);

	if (pl_select(max_fd+1, (read_op) ? &select_fds : NULL, 
			(!read_op) ? &select_fds : NULL, NULL, NULL) == -1) {
		PL_LOG_ERROR("Select failed, errno=%d", errno);
		return -1;
	}
#ifdef CLI
	if (FD_ISSET(0, &select_fds)) {
		process_cli(0);
	}
#endif  // CLI
	/* check for new connection */
	if (FD_ISSET(listen_sock, &select_fds)) {
		struct sockaddr_in mgr_addr;
		socklen_t mgr_addr_len = sizeof(mgr_addr);
		if ((new_sock = pl_accept(listen_sock,
				(struct sockaddr *)&mgr_addr,
				&mgr_addr_len)) == -1) {
			PL_LOG_ERROR("Failed to accept conn, errno=%d", errno);
			return -1;
		}
		PL_LOG_INFO("New conn %d from 0x%08x", new_sock,
				mgr_addr.sin_addr.s_addr);
		if (set_nonblock(new_sock)) {
			pl_close(new_sock);
			return -1;
		}
		*mgr_sock = new_sock;
		return 0;
	} else if ((*mgr_sock != -1) && FD_ISSET(*mgr_sock, &select_fds)) {
		PL_LOG_DEBUG("conn %d ready for %d", *mgr_sock, read_op);
		*ready = true;
		return 0;
	}
	return 0;
}

int
pl_upload_schema_file(struct pl_tftp_server *srv)
{
	PL_LOG_INFO("Upload schema file ... ");
	//TODO: Upload schema file
	return 0;
}

int
pl_download_sw_image(struct pl_tftp_server *srv)
{
	PL_LOG_ERROR("Download SW image ... ");
	//TODO: Download SW Image
	return 0;
}

int
pl_update_sw()
{
	PL_LOG_ERROR("Update SW ... ");
	//TODO: Update SW
	return 0;
}

int
pl_get_sw_update_status()
{
	PL_LOG_ERROR("Get SW update status ... ");
	//TODO: Get SW update status
	return 0;
}

int
pl_printf(void *pr_handle, const char *fmt, ...)
{
	va_list arglist;
	va_start(arglist, fmt);
	vfprintf((FILE *)pr_handle, fmt, arglist);
	va_end(arglist);
}

void
pl_restart_process(uint16_t port)
{
	
	char *exe_name = NULL;
	char *exe_name_dup = NULL;
	size_t len = 0;
	ssize_t read;
	char port_str[32];

	FILE *fp = fopen("/proc/self/exename", "rb");

	if (fp == NULL) {
		PL_LOG_ERROR("Failed to get exe name");
		exit(0);
		return;
	}

	if ((read = getline(&exe_name, &len, fp)) != -1) {
		PL_LOG_INFO("Exename %s", exe_name);
		exe_name_dup = strdup(exe_name);
		sprintf(port_str, "%u", port);
		execl(exe_name, basename(exe_name_dup), port_str, NULL);
	}
	exit(0);
}

