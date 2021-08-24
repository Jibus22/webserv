#include "webserv.hpp"

/*
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
*/

//Create all IP:PORT sockets. Socket descriptor are stored into net_socks
/*
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
*/

static int	create_listening_socket(const std::pair<std::string, int> & ipport)
{
	int					socket_fd, on = 1;
	struct sockaddr_in	sockaddr;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		return sys_err("failed to create a socket: ");

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		return sys_err("failed to set socket option: ");

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(ipport.second);
	sockaddr.sin_addr.s_addr = inet_addr(ipport.first.c_str());
	if (bind(socket_fd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) == -1)
		return sys_err("failed to bind the socket: ");

	if (listen(socket_fd, 5))
		return sys_err("failed to listen the socket: ");
	return socket_fd;
}

//1st, creates unique list of ip:port pair
//2nd, creates listening sockets + map of socket:pair(ip, port)
std::map<int, std::pair<std::string, int> >*
create_network_sockets(const std::vector<SiServ> & srvs)
{
	int												fd;
	std::list<std::pair<std::string, int> >			listen_list;
	std::map<int, std::pair<std::string, int> >
		*server_map = new std::map<int, std::pair<std::string, int> >;

	for (std::vector<SiServ>::const_iterator i = srvs.begin();
			i != srvs.end(); i++)
	{
		listen_list.push_back(i->listen);
	}
	listen_list.sort();
	listen_list.unique();

	for (std::list<std::pair<std::string, int> >::iterator
			i = listen_list.begin(); i != listen_list.end(); i++)
	{
		fd = create_listening_socket(*i);
		if (fd == -1)
		{
			close_server_sockets(*server_map, -1);
			delete server_map;
			return NULL;
		}
		(*server_map)[fd] = *i;
	}
	return server_map;
}
