#include "webserv.hpp"

//utility to close all file descriptors stored into a vector
int	close_listening_ports(const std::vector<int> & net_socks, int ret)
{
	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
		close(*i);
	return ret;
}

int	close_server_sockets(std::map<int, std::pair<std::string, int> >&
			server_map, const int ret)
{
	for (std::map<int, std::pair<std::string, int> >::iterator i
			= server_map.begin(); i != server_map.end(); i++)
	{
		close(i->first);
		server_map.erase(i);
	}
	return ret;
}
