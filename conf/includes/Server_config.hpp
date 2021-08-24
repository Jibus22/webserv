

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include "Config_struct.hpp"

struct Location_config;

struct Server_config
{//------------------> CONSTRUCT <------------------
	Server_config();
	~Server_config();

//------------------> TYPE DEF <-------------------
	typedef Config_struct::c_name_vector			c_name_vector;
	typedef Config_struct::c_methode_vector			c_methode_vector;
	typedef Config_struct::c_loc_map 				c_loc_map;
	typedef Config_struct::c_error_map				c_error_map;
	typedef	Config_struct::p_listen					p_listen;
	
//------------------> VARIABLE <-------------------
	p_listen				listen;
	c_name_vector			name_serv;
	c_error_map				error_page;
	c_loc_map				location;
	size_t			m_body_size;

};

#endif