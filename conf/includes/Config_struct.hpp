

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
	Config_struct();
	~Config_struct();

//------------------> TYPE DEF <-------------------
	typedef std::vector<std::string>					c_name_vector;
	typedef std::vector<std::string>					c_methode_vector;

	typedef std::vector<std::string>					c_index_vector;
	typedef std::vector<Server_config *>				c_serv_vector;
	
	typedef std::vector<Location_config *> 				c_location_vector;




	typedef std::map<std::string, std::string>			c_cgi_map;
	typedef std::map<int, std::string>					c_error_map; 						
	typedef std::pair <std::string, int>				p_listen;
	
//------------------> VARIABLE <------------------
	c_serv_vector	*_main_server;
};

std::ostream& operator<<(std::ostream& os, Location_config &ref);
std::ostream& operator<<(std::ostream& os, Server_config &ref);
std::ostream& operator<<(std::ostream& os, Config_struct  &ref);

# endif
