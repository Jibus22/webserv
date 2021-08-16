

#ifndef	LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

#include "Config_struct.hpp"

struct Location_config
{
//------------------> CONSTRUCTOR <------------------
	Location_config();

//------------------> TYPE DEF <------------------
	typedef Config_struct::c_loc_map				c_loc_map;
	typedef Config_struct::c_str_vector				c_str_vector;
	typedef Config_struct::c_cgi_map				c_cgi_map;
	typedef Config_struct::c_methode_vector			c_methode_vector;
	typedef Config_struct::c_host_vector			c_host_vector;

	typedef Config_struct::serv_port				serv_port;
	typedef Config_struct::c_serv_map				c_serv_nmap;
//------------------> VARIABLE <-------------------
	
	c_str_vector			index;
	c_cgi_map				cgi;
	c_methode_vector		methode;


	std::string				root;
	std::string				uri;
	bool					auto_index;

};

# endif