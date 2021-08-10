#include "webserv.hpp"

int	sys_err(const std::string& msg)
{std::cerr << msg << std::string(strerror(errno)) << std::endl; return -1;}
