#include "webserv.hpp"

//Set all our IP:PORT listening sockets in the kqueue so they are monitored
//for read event
static int	monitor_network_sockets(const int & kq,
		const std::vector<int> & net_socks)
{
	struct kevent	changelist;

	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
	{
		EV_SET(&changelist, *i, EVFILT_READ, EV_ADD, 0, 0, 0);
		if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
			return sys_err("network socks kevent failed");
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

static int	accept_new_client(const int & kq, const int & event_fd)
{
	struct kevent	changelist;
	const int		client_fd = accept(event_fd, NULL, NULL);

	if (client_fd == -1)
		return sys_err("failed to accept a new client");
	EV_SET(&changelist, client_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
	if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
		return sys_err("kevent client_fd failed");
	__D_DISPLAY(" new client had been accepted on socket "
		<< client_fd << " and is now monitored");
	return 0;
}

//Server engine. System is set to notify any event on any network
//socket descriptors & client connection descriptor
int	run_darwin_server(const std::vector<int> & net_socks)
{
	const int		kq = kqueue();
	int				i, event_fd, new_events;
	struct kevent	eventlist[32];

	if (monitor_network_sockets(kq, net_socks) == -1)
		return -1;
	for (;;)//Main loop to wait new events
	{
		__D_DISPLAY("waiting new event...");
		new_events = kevent(kq, NULL, 0, eventlist, 32, NULL);
		if (new_events == -1)
			return sys_err("kevent new event failed");
		for (i = 0; i < new_events; i++)//loop to process triggered events
		{
			event_fd = eventlist[i].ident;
			if (eventlist[i].flags & EV_EOF)
			{
				__D_DISPLAY(" client " << event_fd << " has disconnected");
				close(event_fd);
			}
			else if (check_new_connection(event_fd, net_socks) >= 0)
			{
				if (accept_new_client(kq, event_fd) == -1)
					return -1;
			}
			else if (eventlist[i].filter & EVFILT_READ)//2.
			{
				std::string	str;
				char		buf[RCV_BUF + 1];
				ssize_t		len;

				len = recv(event_fd, buf, RCV_BUF, 0);
				buf[len] = 0;
				str = buf;
				__D_DISPLAY(" client " << event_fd << ": " << len
						<< " bytes had been read: " << str);
			}
		}
	}
	close(kq);
	return 0;
}
