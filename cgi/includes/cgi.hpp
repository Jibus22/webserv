#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"
#include "CgiEnv.hpp"
#include "FtPipe.hpp"

int		process_cgi(Response & response,
				const Request& request,
				const Location_config& location_block,
				const Server_config& server_block,
				Client& client,
				const std::string & cgi_ext,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map);

int		cgi_output(std::string& cgi_out);

#endif
