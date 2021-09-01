#ifndef CGIMETAVAR_HPP
#define CGIMETAVAR_HPP

#include <string>

struct CgiMetaVar
{
	std::string	_server_protocol;
	std::string	_server_software;
	std::string	_gateway_interface;
	std::string	_content_lenght;
	std::string	_content_type;
	std::string	_path_info;
	std::string	_query_string;
	std::string	_request_method;
	std::string	_remote_addr;
	std::string	_script_name;
	std::string	_server_name;
	std::string	_server_port;

	CgiMetaVar();
	CgiMetaVar(CgiMetaVar const & src);
	~CgiMetaVar();
	CgiMetaVar&	operator=(CgiMetaVar const & src);
};

std::ostream &operator<<(std::ostream &out, CgiMetaVar const &value);

#endif
