#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"
#include "CgiEnv.hpp"

void	process_cgi(Response & response,
					const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client,
					const std::string & cgi_ext);

#endif
