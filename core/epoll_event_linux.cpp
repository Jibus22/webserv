#include "webserv.hpp"

//Add read event to kqueue in order to monitor new data in socket_fd read buffer
//While data will be available to read on the socket buffer, we will be
//triggered. When there isn't any data, no trigger: block.
int	add_read_event(const int kq, const int socket_fd)
{
	struct epoll_event	changelist;

	changelist.events = EPOLLIN;
	changelist.data.fd = socket_fd;
	if (epoll_ctl(kq, EPOLL_CTL_ADD, socket_fd, &changelist) == -1)
		return sys_err("epoll_ctl add read event failed. ");
	return 0;
}

//Add oneshot write event to kqueue in order to monitor when client write buffer
//is available (not full). EV_ONESHOT is to clear & delete this event after
//the first trigger.
//This is an edge-triggered event, to avoid this event to never stop triggering,
//(a write buffer is almost always empty)
int	set_write_ready(const int kq, Client& client)
{
	struct epoll_event	changelist;

	changelist.events = EPOLLOUT | EPOLLIN;
	changelist.data.fd = client.getFd();
	if (epoll_ctl(kq, EPOLL_CTL_MOD, client.getFd(), &changelist) == -1)
		return sys_err("epoll_ctl add write event failed. ");
	client.setReady();
	return READY;
}

int	del_event(const int kq, const int client_fd)
{
	if (epoll_ctl(kq, EPOLL_CTL_DEL, client_fd, NULL) == -1)
		return sys_err("delete client epoll_ctl fd failed. ");
	return 0;
}

int	remove_write_event(const int kq, const int client_fd)
{
	struct epoll_event	changelist;

	changelist.events = EPOLLOUT;
	changelist.data.fd = client_fd;
	if (epoll_ctl(kq, EPOLL_CTL_MOD, client_fd, &changelist) == -1)
		return sys_err("epoll_ctl add write event failed. ");
	return 0;
}
