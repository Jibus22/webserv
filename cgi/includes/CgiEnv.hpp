#ifndef CGIENV_HPP
#define CGIENV_HPP

#include "Request.hpp"
#include <string>
#include <vector>

class CgiEnv
{
	const std::string	_server_protocol;
	const std::string	_server_software;
	const std::string	_gateway_interface;
	const std::string	_content_lenght;
	const std::string	_content_type;
	const std::string	_path_info;
	const std::string	_query_string;
	const std::string	_request_method;
	const std::string	_remote_addr;
	const std::string	_script_name;
	const std::string	_server_name;
	const std::string	_server_port;
public:
						CgiEnv();
						CgiEnv(CgiEnv const & src);
						~CgiEnv();
	CgiEnv &			operator=(CgiEnv const & src);

	std::string			getName() const;
};

std::ostream &operator<<(std::ostream &out, CgiEnv const &value);

#endif
