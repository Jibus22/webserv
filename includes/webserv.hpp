#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "system_macro.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#if __APPLE__
#include <sys/event.h>
#else
#include <sys/epoll.h>
#endif

#include "macros.hpp"

#include "core.hpp"
#include "conf.hpp"
#include "processing.hpp"
#include "utils.hpp"

#endif
