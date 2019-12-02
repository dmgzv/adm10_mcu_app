//#include <assert.h>
//#include <stdio.h>
#include <string.h>
//#include <unistd.h>
//#include <errno.h>
//#include <fcntl.h>
#include "lwip/sockets.h"
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include "dev_agent_log.h"
#include "ospl.h"

#define LISTEN_BACKLOG 1

/* set/unset filedescriptor to non-blocking */
static int
set_nonblock(int fd)
{
	int val;

	val = fcntl(fd, F_GETFL,0);
	if (val == -1) {
		LOG_ERROR("fcntl(%d, F_GETFL) failed, errno=%d", fd, errno);
		return -1;
	}
	if (val & O_NONBLOCK) {
		return 0;
	}
	val |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, val) == -1) {
		LOG_ERROR("Failed to set %d to O_NONBLOCK, errno=%d",
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

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		LOG_ERROR("setsockopt SO_REUSEADDR fd %d: %d", fd, errno);
		return -1;
	}
	return 0;
}

/*
 * Initialize listening socket.
 * Returns the listen socket if no error. On error returns -1
 */
int
os_init_listen_sock(in_addr_t addr, uint16_t port)
{
	int listen_sock = -1;
	struct sockaddr_in agt_addr;

	if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		LOG_ERROR("Failed to open socket, errno=%d.", errno);
		return -1;
	}
	LOG_DEBUG("listen_sock=%d", listen_sock);

	if (set_nonblock(listen_sock)) {
		close(listen_sock);
		return -1;
	}
	/*
	if (fcntl(listen_sock, F_SETFD, FD_CLOEXEC) == -1) {
		LOG_ERROR("socket: CLOEXEC: %d", errno);
		close(listen_sock);
		return -1;
	}
	*/
	/* Socket options */
	//if (set_reuseaddr(listen_sock) == -1)
	//	return -1;

	memset(&agt_addr, 0, sizeof(agt_addr));

	agt_addr.sin_family = AF_INET;
	agt_addr.sin_addr.s_addr = htonl(addr);
	agt_addr.sin_port = htons(port);

	if (bind(listen_sock, (struct sockaddr *)&agt_addr,
				sizeof(agt_addr)) == -1) {
		LOG_ERROR("Failed to bind agent port %d, errno=%d.",
				port, errno)
		return -1;
	}

	if (listen(listen_sock, LISTEN_BACKLOG) == -1) {
		LOG_ERROR("Failed to listen, errno=%d.", errno)
		return -1;
	}

	return listen_sock;
}

int
select_one(int listen_sock, int *mgr_sock, bool read, bool *ready)
{
	//assert(listen_sock >= 0);
	//assert(mgr_sock != NULL);
	//assert(ready != NULL);

	fd_set select_fds;
	int max_fd = FD_SETSIZE;
	int new_sock = -1;

	*ready = false;

	LOG_VERBOSE("listen_sock=%d, mgr_sock=%d", listen_sock, *mgr_sock);

	FD_ZERO(&select_fds);
	FD_SET(listen_sock, &select_fds);
	if (*mgr_sock != -1)
		FD_SET(*mgr_sock, &select_fds);
	max_fd = (listen_sock > *mgr_sock) ? listen_sock : *mgr_sock;
	// LOG_VERBOSE("max_fd=%d", max_fd);

	if (select(max_fd+1, (read) ? &select_fds : NULL, 
			(!read) ? &select_fds : NULL, NULL, NULL) == -1) {
		LOG_ERROR("Select failed, errno=%d", errno);
		return -1;
	}
	/* check for new connection */
	if (FD_ISSET(listen_sock, &select_fds)) {
		struct sockaddr_in mgr_addr;
		socklen_t mgr_addr_len = sizeof(mgr_addr);
		if ((new_sock = accept(listen_sock,
				(struct sockaddr *)&mgr_addr,
				&mgr_addr_len)) == -1) {
			LOG_ERROR("Failed to accept conn, errno=%d", errno);
			return -1;
		}
		LOG_INFO("New conn %d from %x", new_sock,
				mgr_addr.sin_addr.s_addr);
		if (set_nonblock(new_sock)) {
			close(new_sock);
			return -1;
		}
		*mgr_sock = new_sock;
		return 0;
	} else if ((*mgr_sock != -1) && FD_ISSET(*mgr_sock, &select_fds)) {
		LOG_DEBUG("conn %d ready for %d", *mgr_sock, read);
		*ready = true;
		return 0;
	}
	return 0;
}

int
upload_schema_file(struct tftp_server *srv) {
	LOG_ERROR("Upload schema file is not implemented");
	//TODO: Upload schema file
	return 0;
}
