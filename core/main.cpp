#include "webserv.hpp"

static std::string	get_webserv_usage()
{
	return std::string("WEBSERV 0.1\nUsage:  ./webserv\n\t"
			"./webserv CONFIGFILE\n\nWhen no config file is provided,"
			" the default config file is used.\n"
			"Default config file: $HOME/webserv.conf");
}

static void	get_default_conf_file(std::string& file)
{
	const char	*home;

	home = getenv("HOME");
	if (!home)
		return ;
	file.assign(home);
	file.append("/webserv.conf");
}

static std::vector<Server_config *>
*load_configuration(int ac, char *av[])
{
	std::vector<Server_config *>	*conf = NULL;
	std::string						file;
	Config_base						*parser;

	if (ac == 2)
	{
		file = std::string(av[1]);
		parser = new Config_base(file);
		conf = parser->get_vector();
		delete parser;
	}
	else if (_DEBUG)
		conf = get_servers_simulation();
	else
	{
		get_default_conf_file(file);
		if (!is_openable(file))
			return static_cast<std::vector<Server_config *> *>
				(pgm_err2(get_webserv_usage()));
		parser = new Config_base(file);
		conf = parser->get_vector();
		delete parser;
	}
	return conf;
}

int	main(int ac, char *av[])
{
	std::vector<Server_config *>				*conf;
	std::map<int, std::pair<std::string, int> >	*server_map;

	conf = load_configuration(ac, av);
	if (!conf)
		return 1;
	server_map = create_network_sockets(*conf);
	if (server_map)
		start_server(*conf, *server_map);
	close_server_sockets(*server_map, 0);
	delete server_map;
	delete conf;
	return 0;
}
