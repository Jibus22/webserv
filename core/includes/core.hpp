#ifndef CORE_HPP
#define CORE_HPP

#include <map>
#include "SiServ.hpp"

//__________________________________PROTOTYPES________________________________//
//______CORE_______//
std::map<int, std::pair<std::string, int> >*
		create_network_sockets(const std::vector<SiServ> & srvs);
int		start_server(const std::vector<SiServ> & server_blocks,
					std::map<int, std::pair<std::string, int> >	& server_map);


#endif
