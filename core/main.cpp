#include "webserv.hpp"

int	sys_err(const std::string& msg)
{std::cerr << msg << std::string(strerror(errno)) << std::endl; return -1;}

//socket creation for any network communication endpoint as IP:PORT format
int	create_listening_socket(const int port)
{
	int					socket_fd, on = 1;
	struct sockaddr_in	sockaddr;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		return sys_err("failed to create a socket: ");

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		return sys_err("failed to set socket option: ");

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (bind(socket_fd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) == -1)
		return sys_err("failed to bind the socket: ");

	if (listen(socket_fd, 5))
		return sys_err("failed to listen the socket: ");
	return socket_fd;
}

//Set all our IP:PORT listening sockets in the kqueue so they are monitored
//for read event
int	kmonitor_network_sockets(const int & kq, const std::vector<int> & net_socks)
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
int	check_new_connection(const int & event_fd,
		const std::vector<int> & net_socks)
{
	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
		if (*i == event_fd)
			return event_fd;
	return -1;
}

int	kaccept_new_client(const int & kq, const int & event_fd)
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

	if (kmonitor_network_sockets(kq, net_socks) == -1)
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
				if (kaccept_new_client(kq, event_fd) == -1)
					return -1;
			}
			else if (eventlist[i].filter & EVFILT_READ)//2.
			{
				char	buf[5];
				memset(buf, 0, sizeof(buf));
				ssize_t	len = recv(event_fd, buf, 4, 0);
				__D_DISPLAY(" client " << event_fd << ": " << len
						<< " bytes had been read: " << std::string(buf));
			}
		}
	}
	return 0;
}

//temporary function to simulate all ports given back by the file parsing
std::vector<int>	bunch_of_ports_simulation()
{
	std::vector<int>	ports;
	int					nb = 8080;

	for (int i = 0; i < 6; i++, nb++)
		ports.push_back(nb);
	return ports;
}

//utility to close all file descriptors stored into a vector
int	close_listening_ports(const std::vector<int> & net_socks, int ret)
{
	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
		close(*i);
	return ret;
}

//Create all IP:PORT sockets. Socket descriptor are stored into net_socks
int	create_network_sockets(const std::vector<int> & ports,
			std::vector<int> & net_socks)
{
	int	fd;

	net_socks.reserve(ports.size());
	for (std::vector<int>::const_iterator i = ports.begin();
			i != ports.end(); i++)
	{
		fd = create_listening_socket(*i);
		if (fd == -1)
			return close_listening_ports(net_socks, -1);
		net_socks.push_back(fd);
	}
	return 0;
}

int	main(void)
{
	std::vector<int>	ports(bunch_of_ports_simulation());
	std::vector<int>	network_sockets;

	if (create_network_sockets(ports, network_sockets) == -1)
		return -1;
	if (__APPLE__)
		run_darwin_server(network_sockets);
	else
		std::cout << "kernel isn't darwin" << std::endl;
	close_listening_ports(network_sockets, 0);

	return 0;
}
