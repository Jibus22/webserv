#ifndef UTILS_HPP
#define UTILS_HPP

//__________________________________PROTOTYPES________________________________//
//______UTILS______//
int		sys_err(const std::string& msg);
int		pgm_err(const std::string& msg);
int		pgm_perr(const std::string& msg);
int		http_error(Client& client, const std::string& msg, int ret);

int		close_server_sockets(std::map<int, std::pair<std::string, int> >&
			server_map, const int ret);
void	close_server_sockets(const std::map<int, std::pair<std::string, int> >&
			server_map);
void	close_client_sockets(const std::map<int, Client>& client_map);

std::vector<Server_config *>	*get_servers_simulation();

long	get_timestamp(const struct timeval& tv_start);
int		ft_string_to_nb(const std::string& value);
std::string	ft_int_to_string(int val);

size_t	find_nocase_header(const std::string& str, const std::string& header);

int		is_file_exist(const char *filename);
int		is_file_exist(const std::string& filename);

//display.cpp
const std::string	display_headers(const std::string& response);
void				__D_DISPLAY_RECV(int client_id, int len);
void				__D_DISPLAY_SEND(int client_id, int len,
						int send_id, const std::string& response);

#endif
