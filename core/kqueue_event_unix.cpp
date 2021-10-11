#include "webserv.hpp"

//Add read event to kqueue in order to monitor new data in socket_fd read buffer
//While data will be available to read on the socket buffer, we will be
//triggered. When there isn't any data, no trigger: block.
int	add_read_event(const int kq, const int socket_fd)
{
	struct kevent	changelist;

	memset(&changelist, 0, sizeof(changelist));
	EV_SET(&changelist, socket_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
	if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
		return sys_err("kevent client_fd failed");
	return 0;
}

//Add oneshot write event to kqueue in order to monitor when client write buffer
//is available (not full). EV_ONESHOT is to clear & delete this event after
//the first trigger.
//This is an edge-triggered event, to avoid this event to never stop triggering,
//(a write buffer is almost always empty)
int	set_write_ready(const int kq, Client& client)
{
	struct kevent	changelist;

	memset(&changelist, 0, sizeof(changelist));
	EV_SET(&changelist, client.getFd(), EVFILT_WRITE,
			EV_ADD | EV_ONESHOT, 0, 0, 0);
	if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
		return sys_err("kevent client_fd failed");
	client.setReady();
	return READY;
}
