

#ifndef	CONFIG_STRUCT_HPP
# define CONFIG_STRUCT_HPP

# include <iostream>
# include <string>
# include <sstream>
# include <fstream>
# include <exception>
# include <stdexcept>
# include <algorithm>
# include <arpa/inet.h>

# include <map>
# include <list>
# include <vector>

struct Server_config;
struct Location_config;

struct Config_struct
{//------------------> CONSTRUCT <------------------
	~Config_struct();

//------------------> TYPE DEF <-------------------
	typedef std::map<std::string, Location_config *>	c_loc_map;
	typedef std::vector<std::string>					c_str_vector;
	typedef std::map<std::string, std::string>			c_cgi_map;
	typedef std::vector<std::string>					c_methode_vector;
	typedef std::vector<std::string>					c_host_vector;

	typedef int											serv_port;	
	typedef std::list<Server_config *>					c_serv_list;
	typedef std::map<serv_port, c_serv_list>			c_serv_map;

//------------------> VARIABLE <------------------
	c_serv_map	_main_server;	

};

std::ostream& operator<<(std::ostream& os, Location_config &ref);
std::ostream& operator<<(std::ostream& os, Server_config &ref);
std::ostream& operator<<(std::ostream& os, Config_struct  &ref);



# endif
