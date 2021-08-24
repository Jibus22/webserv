#include "webserv.hpp"

void
getPorts(const std::map<int, std::list<Server_config *> >& conf,
		std::vector<int>& ports)
{
	typedef std::map<int, std::list<Server_config *> >::const_iterator
		const_iterator;

	for (const_iterator i = conf.begin(); i != conf.end(); i++)
	{
		ports.push_back(i->first);
	}
}

Config_struct	*handle_config(const int& ac, char *av[],
					std::vector<int>& ports)
{
	Config_base			*conf_b;
	Config_struct		*conf;
	std::string			file_config;

	if (ac != 2)
	{
		if (_DEBUG)
			bunch_of_ports_simulation(ports);
		else
			pgm_err("conf file missing");
		return NULL;
	}
	else
	{
 		file_config = av[1]; 
		conf_b = new Config_base(file_config);
		conf = new Config_struct();
		*conf = conf_b->parsing_return();
		delete conf_b;
		getPorts(conf->_main_server, ports);
	}
	return conf;
}

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
