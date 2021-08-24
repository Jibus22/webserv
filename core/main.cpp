#include "webserv.hpp"

void	display_servermap(std::map<int, std::pair<std::string, int> >& srvmp)
{
	__D_DISPLAY("Display map of servers");
	for (std::map<int, std::pair<std::string, int> >::const_iterator i
			= srvmp.begin(); i != srvmp.end(); i++)
	{
		__D_DISPLAY("fd: " << i->first << " - ip: " << i->second.first
				<< " - port: " << i->second.second);
	}
}

int	main(int ac, char *av[])
{
	std::vector<SiServ>							*conf;
	std::map<int, std::pair<std::string, int> >	*server_map;

	conf = get_servers_simulation();
	server_map = create_network_sockets(*conf);
	if (server_map == NULL)
		return -1;
	display_servermap(*server_map);
	start_server(*conf, *server_map);
	close_server_sockets(*server_map, 0);
	delete server_map;
	delete conf;
	(void)ac;
	(void)av;

	return 0;
}
