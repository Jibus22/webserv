#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

void	process_request(Client& client,
				const std::vector<Server_config*>& server_blocks);

#endif
