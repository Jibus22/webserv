#ifndef CORE_HPP
#define CORE_HPP

#include <map>
#include "Client.hpp"
#include "conf.hpp"

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

//--server_process.cpp--
int		is_response(const int kq, const struct kevent *event,
					std::map<int, Client>& client_map);
int		send_response(const int kq, const struct kevent *event, Client& client);
int		read_request(const int event_fd,
				const std::vector<Server_config*>& server_blocks,
					std::map<int, Client>& client_map);

int		accept_new_client(const int kq, const int event_fd,
			std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map);

int		check_new_connection(const int event_fd,
			const std::map<int, std::pair<std::string, int> >& server_map);

int		monitor_network_sockets(const int kq,
				const std::map<int, std::pair<std::string, int> > & server_map);


#endif
