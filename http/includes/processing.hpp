#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "SiServ.hpp"

class Server {};

std::string const *process_request(std::string& raw_request,
	const std::vector<Server>& server_blocks,
	const std::pair<std::string, int>& listen, int *flag);

void	process_request(Client& client,
				const std::vector<SiServ>& server_blocks);

#endif
