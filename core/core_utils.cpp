#include "webserv.hpp"

void	remove_client(std::map<int, Client>& client_map, int client_fd)
{
	__D_DISPLAY(" client " << client_fd << " has disconnected");
	close(client_fd);
	client_map.erase(client_fd);
}

void	remove_all_clients(std::map<int, Client>& client_map)
{
	for (std::map<int, Client>::const_iterator it = client_map.begin();
			it != client_map.end(); it++)
		close(it->first);
	client_map.clear();
}

int		check_return(int ret, std::map<int, Client>& client_map)
{
	if (ret == -1)
	{
		if (errno == EMFILE || errno == ENFILE)
			remove_all_clients(client_map);
		else
			return sys_err("failed to accept a new client");
	}
	return 0;
}
