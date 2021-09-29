#include "webserv.hpp"

int	http_response(Client& client, int http_status, int ret)
{
	std::string	*response = new std::string;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	if (http_status == 200)
		response->append(" OK\r\n");
	else if (http_status == 201)
		response->append(" Created\r\n");
	else
		response->append(" Kamoulox\r\n");
	response->append("Content-Length:0\r\n\r\n");
	client.setResponse(response);
	return ret;
}
