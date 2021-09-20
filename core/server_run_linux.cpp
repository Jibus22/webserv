#include "webserv.hpp"

//Set all our IP:PORT listening sockets in the kqueue so they are monitored
//for read event
static int	monitor_network_sockets(const int & ep,
		const std::vector<int> & net_socks)
{
	struct epoll_event	changelist;

	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
	{
		changelist.events = EPOLLIN;
		changelist.data.fd = *i;
		if (epoll_ctl(ep, EPOLL_CTL_ADD, *i, &changelist) == -1)
			return sys_err("network socks epoll_ctl failed");
	}
	return 0;
}

//check if 'event_fd == one of network fd from vector', which would means
//a new client wanna connect.
static int	check_new_connection(const int & event_fd,
		const std::vector<int> & net_socks)
{
	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
		if (*i == event_fd)
			return event_fd;
	return -1;
}

static int	accept_new_client(const int & ep, const int & event_fd)
{
	struct epoll_event	changelist;
	const int			client_fd = accept(event_fd, NULL, NULL);

	if (client_fd == -1)
		return sys_err("failed to accept a new client");
	changelist.events = EPOLLIN;
	changelist.data.fd = client_fd;
	if (epoll_ctl(ep, EPOLL_CTL_ADD, client_fd, &changelist) == -1)
		return sys_err("network socks epoll_ctl failed");
	__D_DISPLAY(" new client had been accepted on socket "
		<< client_fd << " and is now monitored");
	return 0;
}

//Server engine. System is set to notify any event on any network
//socket descriptors & client connection descriptor
int	run_darwin_server(const std::vector<int> & net_socks)
{
	const int			ep = epoll_create(1);
	int					i, event_fd, new_events, disconnected = 0;
	struct epoll_event	eventlist[32];

	if (monitor_network_sockets(ep, net_socks) == -1)
		return -1;
	for (;;)//Main loop to wait new events
	{
		__D_DISPLAY("waiting new event...");
		new_events = epoll_wait(ep, eventlist, 32, -1);
		if (new_events == -1)
			return sys_err("kevent new event failed");
		for (i = 0; i < new_events; i++)//loop to process triggered events
		{
			event_fd = eventlist[i].data.fd;
			if (eventlist[i].events & EPOLLRDHUP
					|| eventlist[i].events & EPOLLHUP
					|| eventlist[i].events & EPOLLERR
					|| !(eventlist[i].events & EPOLLIN))
				disconnected = 1;
			else if (check_new_connection(event_fd, net_socks) >= 0)
			{
				if (accept_new_client(ep, event_fd) == -1)
					return -1;
			}
			else if (eventlist[i].events & EPOLLIN)//2.
			{
				char	buf[5];
				ssize_t	len;

				memset(buf, 0, sizeof(buf));
				len = recv(event_fd, buf, 4, 0);
				if (!len)
					disconnected = 1;
				else {
					__D_DISPLAY(" client " << event_fd << ": " << len
						<< " bytes had been read: " << std::string(buf));
				}
			}
			if (disconnected)
			{
				__D_DISPLAY(" client " << event_fd << " has disconnected");
				if (epoll_ctl(ep, EPOLL_CTL_DEL, event_fd, NULL) == -1)
					return sys_err("delete client epoll_ctl fd failed");
				close(event_fd);
				disconnected = 0;
			}
		}
	}
	close(ep);
	return 0;
}
