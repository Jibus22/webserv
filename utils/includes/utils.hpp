#ifndef UTILS_HPP
#define UTILS_HPP

//__________________________________PROTOTYPES________________________________//
//errors.cpp
int		sys_err(const std::string& msg);
int		pgm_err(const std::string& msg);
int		pgm_perr(const std::string& msg);
int		http_error(Client& client, const std::string& msg, int ret);

//close_fd.cpp
int		close_server_sockets(std::map<int, std::pair<std::string, int> >&
			server_map, const int ret);
void	close_server_sockets(const std::map<int, std::pair<std::string, int> >&
			server_map);
void	close_client_sockets(const std::map<int, Client>& client_map);

//simulations.cpp
std::vector<Server_config *>	*get_servers_simulation();


//find_nocase_header.cpp
//size_t	find_nocase_header(const std::string& str, const std::string& header);
size_t	find_nocase_header(const std::string& str,
					const std::string& header, size_t start = 0);

//utils.cpp
long		get_timestamp(const struct timeval& tv_start);
int			ft_string_to_nb(const std::string& value);
std::string	ft_int_to_string(int val);
int			is_file_exist(const char *filename);
int			is_file_exist(const std::string& filename);
std::string	str_to_lower(std::string const & s);

//display.cpp
const std::string	display_headers(const std::string& response);
void				__D_DISPLAY_RECV(int client_id, int len);
void				__D_DISPLAY_SEND(int client_id, int len,
						int send_id, const std::string& response);

#endif
