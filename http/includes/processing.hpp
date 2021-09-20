#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

void	process_request(Client& client,
				const std::vector<Server_config *>& server_blocks,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map);

bool is_dir(const std::string path);
bool	get_file_content(std::string const & path, std::string & content);

#endif
