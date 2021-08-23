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

int	main(int ac, char *av[])
{
	std::vector<server>	*conf;

	conf = get_servers_simulation();
	run_darwin_server(*conf);
	close_listening_ports(network_sockets, 0);

	return 0;
}
