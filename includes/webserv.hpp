#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "system_macro.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <ctype.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <queue>

#if __APPLE__
#include <sys/event.h>
#else
#include <sys/epoll.h>
#endif

#include "macros.hpp"

#include "conf.hpp"
#include "core.hpp"
#include "processing.hpp"
#include "utils.hpp"
#include "cgi.hpp"

#endif
