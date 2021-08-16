
# include "includes/Server_config.hpp"

Server_config::Server_config(){
	this->auto_index = false;
}

Server_config::~Server_config(){
}

//------------------> PRINT SERVER NGINX <------------------
std::ostream& operator<<(std::ostream& os, Server_config &ref)
{
	for(Server_config::c_host_vector::iterator it = ref.host.begin(); it != ref.host.end(); it++)
		os << "LISTEN = " << *it << std::endl;
	os << "PORT = " << ref.port << std::endl;
	for (Server_config::c_str_vector::iterator it = ref.name.begin(); it != ref.name.end(); it++)
		os << "SERVEUR NAME = " << *it << std::endl;
	for (Server_config::c_cgi_map::iterator it = ref.cgi.begin(); it != ref.cgi.end(); it++)
		os << "CGI_EXT exec = " << it->first << "\nCGI_EXT chemin = " << it->second << std::endl;
	os << "ROOT = " << ref.root << std::endl;

	for(Server_config::c_str_vector::iterator it = ref.index.begin(); it != ref.index.end(); it++)
		os << "INDEX = " << *it << std::endl;

	for(Server_config::c_loc_map::iterator it = ref.location.begin(); it != ref.location.end(); it++)
		os << "\nLOCATION " << "URI = " << it->first << "\n" << *it->second << "\n";
	
	return (os);
}
