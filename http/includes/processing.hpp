#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include <iostream>
#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "conf.hpp"

void    process_cgi(Response & response,
					const Request& request,
					const Server_config * server_block,
					const Location_config * location_block,
					const std::string & cgi_ext);

void	process_request(Client& client,
				const std::vector<Server_config*>& server_blocks);

#endif
