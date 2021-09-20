#include "webserv.hpp"

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

void	close_server_sockets(const std::map<int, std::pair<std::string, int> >&
			server_map)
{
	for (std::map<int, std::pair<std::string, int> >::const_iterator i
			= server_map.begin(); i != server_map.end(); i++)
	{
		close(i->first);
	}
}

void	close_client_sockets(const std::map<int, Client>& client_map)
{
	for (std::map<int, Client>::const_iterator i
			= client_map.begin(); i != client_map.end(); i++)
	{
		close(i->first);
	}
}
