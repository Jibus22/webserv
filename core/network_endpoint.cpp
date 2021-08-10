#include "webserv.hpp"

//socket creation for any network communication endpoint as IP:PORT format
static int	create_listening_socket(const int port)
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
