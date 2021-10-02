#ifndef PROCESSING_HPP
# define PROCESSING_HPP

#include "webserv.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int		process_request(Client& client,
				const std::vector<Server_config *>& server_blocks,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map);

bool	get_file_content(std::string const & path, std::string & content);
void	auto_index(Response & response, std::string  const & target);

int		formdata_process(Client& client, const std::string& request,
						const std::string& value, const std::string& updir,
						size_t boundary_pos, const Server_config& server,
						const std::string& target);

int		http_error(Client& client, int http_status, int ret);
int		http_error(Client& client, const std::map<int, std::string>& err,
					int http_status, int ret);
int		http_error(Client& client, const std::map<int, std::string>& err,
					const std::string& value, int http_status, int ret);
int		http_response(Client& client, const std::string& headers,
					int http_status, int ret);
int		http_response(Client& client, const std::string& value,
					int http_status, int ret, const std::string& filename,
					size_t filesize);

#endif
