#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include <iostream>
#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include "conf.hpp"

void	process_request(Client& client,
				const std::vector<Server_config*>& server_blocks);

class NoServerMatchException:std::exception
{
	const char* what() const throw();
};


#endif
