#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <sys/event.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <ostream>
#include <cstring>
#include <vector>

#ifndef __APPLE__
# define __APPLE__ 0
#endif

#ifndef _DEBUG
# define _DEBUG 0
#endif

# define __D_DISPLAY(fmt) \
	if (_DEBUG) {\
	std::ostringstream ost;\
	ost << fmt << std::endl;\
	std::cout << ost.str();} else ;\

#endif
