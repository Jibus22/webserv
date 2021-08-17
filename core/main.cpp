#include "webserv.hpp"

//temporary function to simulate all ports given back by the file parsing
void	bunch_of_ports_simulation(std::vector<int>& ports)
{
	int					nb = 8080;

	for (int i = 0; i < 6; i++, nb++)
		ports.push_back(nb);
}

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
	std::vector<int>	ports;
	std::vector<int>	network_sockets;
	Config_struct		*conf;

	conf = handle_config(ac, av, ports);
	if (conf == NULL && !_DEBUG)
		return -1;
	__D_DISPLAY(ports.size());
	if (create_network_sockets(ports, network_sockets) == -1)
		return -1;
	run_darwin_server(network_sockets);
	close_listening_ports(network_sockets, 0);

	return 0;
}
