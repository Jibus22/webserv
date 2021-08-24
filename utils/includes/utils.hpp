#ifndef UTILS_HPP
#define UTILS_HPP

#include "SiServ.hpp"

//__________________________________PROTOTYPES________________________________//
//______UTILS______//
int		sys_err(const std::string& msg);
int		pgm_err(const std::string& msg);
int		close_listening_ports(const std::vector<int> & net_socks, int ret);
int		close_server_sockets(std::map<int, std::pair<std::string, int> >&
			server_map, const int ret);

std::vector<SiServ>	*get_servers_simulation();


#endif
