#include "webserv.hpp"

void    process_cgi(Response & response,
					const Request& request,
					const Server_config * server_block,
					const Location_config * location_block,
					const std::string & cgi_ext)
{
	(void) request;
	(void) response;
	(void) location_block;
	(void) server_block;

	__D_DISPLAY("cgi : " << cgi_ext);
}
