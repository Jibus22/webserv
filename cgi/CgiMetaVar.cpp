#include "CgiMetaVar.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

CgiMetaVar::CgiMetaVar(void) : _server_protocol("SERVER_PROTOCOL = HTTP/1.1"),
						_server_software("SERVER_SOFTWARE = webserv"),
						_gateway_interface("GATEWAY_INTERFACE = CGI/1.1"),
						_content_lenght("CONTENT_LENGHT = "),
						_content_type("CONTENT_TYPE = "),
						_path_info("PATH_INFO = "),
						_query_string("QUERY_STRING = "),
						_request_method("REQUEST_METHOD = "),
						_remote_addr("REMOTE_ADDR = "),
						_script_name("SCRIPT_NAME = "),
						_server_name("SERVER_NAME = "),
						_server_port("SERVER_PORT = ")
{}


CgiMetaVar::CgiMetaVar(const CgiMetaVar& src) :
						_server_protocol(src._server_protocol),
						_server_software(src._server_software),
						_gateway_interface(src._gateway_interface),
						_content_lenght(src._content_lenght),
						_content_type(src._content_type),
						_path_info(src._path_info),
						_query_string(src._query_string),
						_request_method(src._request_method),
						_remote_addr(src._remote_addr),
						_script_name(src._script_name),
						_server_name(src._server_name),
						_server_port(src._server_port)
{}

CgiMetaVar::~CgiMetaVar()
{}

CgiMetaVar &	CgiMetaVar::operator=(const CgiMetaVar& src)
{
	if (this == &src)
		return *this;
	_server_protocol = src._server_protocol;
	_server_software = src._server_software;
	_gateway_interface = src._gateway_interface;
	_content_lenght = src._content_lenght;
	_content_type = src._content_type;
	_path_info = src._path_info;
	_query_string = src._query_string;
	_request_method = src._request_method;
	_remote_addr = src._remote_addr;
	_script_name = src._script_name;
	_server_name = src._server_name;
	_server_port = src._server_port;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/



/*------------------------------GETTERS/SETTERS-------------------------------*/

std::ostream &operator<<(std::ostream &out, CgiMetaVar const &value)
{
	out <<	_server_protocol << std::endl
	<<	_server_software << std::endl
	<<	_gateway_interface << std::endl
	<<	_content_lenght << std::endl
	<<	_content_type << std::endl
	<<	_path_info << std::endl
	<<	_query_string << std::endl
	<<	_request_method << std::endl
	<<	_remote_addr << std::endl
	<<	_script_name << std::endl
	<<	_server_name << std::endl
	<<	_server_port << std::endl;
	return (out);
}
