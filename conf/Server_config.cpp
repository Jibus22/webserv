
# include "includes/Server_config.hpp"

Server_config::Server_config(){
	
}

Server_config::~Server_config(){
}

//------------------> PRINT SERVER NGINX <------------------
std::ostream& operator<<(std::ostream& os, Server_config &ref){
	os << "LISTEN HOST = " << "{" << ref.listen.first << "}" << "\n" << "LISTENT PORT = " << "{" << ref.listen.second << "}" << "\n";

	for (Server_config::c_name_vector::iterator it = ref.name_serv.begin(); it != ref.name_serv.end(); it++)
		os << "SERVEUR NAME = " << "{" << *it << "}" << std::endl;
	
	for (Server_config::c_error_map::iterator it = ref.error_page.begin(); it != ref.error_page.end(); it++)
		os << "ERROR_PAGE NUMBER = " << "{" << it->first << "}" << "\nERROR_PAGE = " << "{" << it->second << "}" << std::endl;

	os << "CLIENT_MAX_BODY = " << "{" << ref.m_body_size << "}" << "\n";

	for(Server_config::c_location_vector::iterator it = ref.location.begin(); it != ref.location.end(); it++)
		os << "\nLOCATION " << **it  << "\n";
	
	return (os);
}
