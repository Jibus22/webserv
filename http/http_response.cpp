#include "webserv.hpp"

static void	set_success_status(std::string& response,
					const std::string& value, int http_status)
{
	if (http_status == 200)
		response.append(" OK\r\n");
	else if (http_status == 201)
		response.append(" Created\r\n");
	else if (http_status == 303)
	{
		response.append(" See Other\r\n");
		response.append("Location:" + value + "\r\n");
	}
	else
		response.append(" Kamoulox\r\n");
}

int	http_response(Client& client, const std::string& value,
					int http_status, int ret)
{
	std::string	*response = new std::string;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	set_success_status(*response, value, http_status);
	response->append("Content-Length:0\r\n\r\n");
	client.setResponse(response);
	client.truncateRequest(client.getRequestSize());
	return ret;
}
