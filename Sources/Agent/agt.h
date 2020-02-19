#ifndef _AGT_H_
#define _AGT_H_

#include <stdint.h>
#include <stdbool.h>

struct agt_buf {
	char buf[AGT_MAX_MSGLEN];
	size_t filled;		// size of req filled in the buf so far
	// In case of large messages filled can be more than AGT_MAX_MSGLEN,
	// but buffer overflow will not be there.
};

struct agt {
	struct agt_buf req_buf;
	struct agt_buf rsp_buf;
	int agt_listen_sock;
	int mgr_sock;
	uint16_t listen_port;
	bool in_trans;
	uint8_t sw_update_state;
};

void agt_run_agent(int listen_sock, struct agt_buf *req_buf,
		struct agt_buf *rsp_buf);

#endif  /* _AGT_H_ */
