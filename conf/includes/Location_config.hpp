

#ifndef	LOCATION_CONFIG_HPP
# define LOCATION_CONFIG_HPP

#include "Config_struct.hpp"

struct Location_config
{
//------------------> CONSTRUCTOR <------------------
	Location_config();

//------------------> TYPE DEF <------------------
	typedef Config_struct::c_methode_vector			c_methode_vector;
	typedef Config_struct::c_error_map				c_error_map;
	typedef Config_struct::c_cgi_map				c_cgi_map;

//------------------> VARIABLE <-------------------
	c_methode_vector		methode;
	c_error_map				error_page;
	c_cgi_map				cgi;
	std::string				index;
	std::string				root;
	std::string				uri;
	std::string				upload_dir;

	bool					auto_index;
};

# endif