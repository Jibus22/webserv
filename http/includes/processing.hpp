#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "SiServ.hpp"
#include <vector>

class Server {};

void	process_request(Client& client,
				const std::vector<SiServ>& server_blocks);

class NoServerMatchException:std::exception
{
	const char* what() const throw();
};

#endif
