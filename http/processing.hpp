#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "Request.hpp"
#include "Response.hpp"

std::string const *process_request(std::string& raw_request,
	const std::vector<Server>& server_blocks,
	const std::pair<std::string, int>& listen, int *flag);

#endif
