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

std::vector<Server_config *>	*load_configuration(int ac, char *av[])
{
	std::vector<Server_config *>	*conf = NULL;
	std::string						*file;
	Config_base						*parser;

	if (ac == 2)
	{
		file = new std::string(av[1]);
		parser = new Config_base(*file);
		conf = parser->get_vector();
		delete file;
		delete parser;
	}
	else if (_DEBUG)
		conf = get_servers_simulation();
	else
		pgm_err("conf file missing");
	return conf;
}

int	main(int ac, char *av[])
{
	std::vector<Server_config *>				*conf;
	std::map<int, std::pair<std::string, int> >	*server_map;

	conf = load_configuration(ac, av);
	server_map = create_network_sockets(*conf);
	if (server_map == NULL)
		return -1;
	display_servermap(*server_map);
	start_server(*conf, *server_map);
	close_server_sockets(*server_map, 0);
	delete server_map;
	delete conf;

	return 0;
}
