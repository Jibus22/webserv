#include "webserv.hpp"

int	sys_err(const std::string& msg)
{std::cerr << msg << std::string(strerror(errno)) << std::endl; return -1;}

int	pgm_err(const std::string& msg)
{std::cerr << msg << std::endl; return -1;}

int	pgm_perr(const std::string& msg)
{perror(msg.c_str()); return -1;}

int	http_error(Client& client, const std::string& msg, int ret)
{
	client.setResponse(new std::string(msg));
	return ret;
}
