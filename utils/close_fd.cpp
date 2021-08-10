#include "webserv.hpp"

//utility to close all file descriptors stored into a vector
int	close_listening_ports(const std::vector<int> & net_socks, int ret)
{
	for (std::vector<int>::const_iterator i = net_socks.begin();
			i != net_socks.end(); i++)
		close(*i);
	return ret;
}
