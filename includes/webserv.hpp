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
#include <vector>
#if __APPLE__
#include <sys/event.h>
#else
#include <sys/epoll.h>
#endif

#include "conf.hpp"



//__________________________________MACROS____________________________________//
# define __D_DISPLAY(fmt) \
	if (_DEBUG) {\
	std::ostringstream ost;\
	ost << fmt << std::endl;\
	std::cout << ost.str();} else ;\



//__________________________________PROTOTYPES________________________________//

//______CORE_______//
int	create_network_sockets(const std::vector<int> & ports,
			std::vector<int> & net_socks);
int	run_darwin_server(const std::vector<int> & net_socks);


//______UTILS______//
int	sys_err(const std::string& msg);
int	pgm_err(const std::string& msg);
int	close_listening_ports(const std::vector<int> & net_socks, int ret);



#endif
