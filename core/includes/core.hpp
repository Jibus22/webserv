#ifndef CORE_HPP
#define CORE_HPP

#include "webserv.hpp"
#include "conf.hpp"
#include "Client.hpp"

//__________________________________PROTOTYPES________________________________//
//--network_endpoint.cpp--
std::map<int, std::pair<std::string, int> >*
		create_network_sockets(const std::vector<Server_config *> & srvs);

//--server_run_osx.cpp--
int		start_server(const std::vector<Server_config*> & server_blocks,
					std::map<int, std::pair<std::string, int> >	& server_map);

//--kqueue_event_osx.cpp--
int		add_read_event(const int kq, const int socket_fd);
int		set_write_ready(const int kq, Client& client);
//--epoll_event_linux.cpp--
int		del_event(const int kq, const int client_fd);
int		remove_write_event(const int kq, const int client_fd);
void	remove_client(std::map<int, Client>& client_map, const int client_fd,
				const int kq);

//--server_io_unix.cpp--
int	send_response(const int kq, const struct kevent& event, Client& client);
int	send_response(const int kq, const struct epoll_event& event, Client& client);
int	read_request(const struct kevent& event, Client& client);
int	read_request(const struct epoll_event& event, Client& client);

//--server_process.cpp--
int	accept_new_client(const int kq, const int event_fd,
			std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map);

int	check_new_connection(const int event_fd,
			const std::map<int, std::pair<std::string, int> >& server_map);
int	monitor_network_sockets(const int kq,
				const std::map<int, std::pair<std::string, int> > & server_map);

int		is_valid_request(Client& client);

//--utils.cpp--
void	remove_all_clients(std::map<int, Client>& client_map);
void	remove_client(std::map<int, Client>& client_map, int client_fd);
int		check_return(int ret, std::map<int, Client>& client_map);

#endif
