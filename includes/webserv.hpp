#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "system_macro.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
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
#include "colorResources.hpp"

#include "conf.hpp"
#include "core.hpp"
#include "processing.hpp"
#include "utils.hpp"
#include "cgi.hpp"

#endif
