#ifndef WEBSERV_HPP
#define WEBSERV_HPP



//__________________________________DEFINES___________________________________//
#ifndef __APPLE__
# define __APPLE__ 0
#endif

#ifndef _DEBUG
# define _DEBUG 0
#endif



//__________________________________HEADERS___________________________________//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <cstring>
#include <string>
#include <vector>
#if __APPLE__
#include <sys/event.h>
#else
#include <sys/epoll.h>
#endif

#include "SiServ.hpp"
#include "Client.hpp"

#include "conf.hpp"



//__________________________________MACROS____________________________________//
# define __D_DISPLAY(fmt) \
	if (_DEBUG) {\
	std::ostringstream ost;\
	ost << fmt << std::endl;\
	std::cout << ost.str();} else ;\

# define RCV_BUF 5
# define INCOMPLETE 1
# define COMPLETE 0


//__________________________________PROTOTYPES________________________________//

//______CORE_______//
std::map<int, std::pair<std::string, int> >*
		create_network_sockets(const std::vector<SiServ> & srvs);
int		start_server(const std::vector<SiServ> & server_blocks,
					std::map<int, std::pair<std::string, int> >	& server_map);


//______UTILS______//
int		sys_err(const std::string& msg);
int		pgm_err(const std::string& msg);
int		close_listening_ports(const std::vector<int> & net_socks, int ret);
int		close_server_sockets(std::map<int, std::pair<std::string, int> >&
			server_map, const int ret);

void					bunch_of_ports_simulation(std::vector<int>& ports);
std::vector<SiServ>	*get_servers_simulation();

#endif
