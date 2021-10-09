#include "webserv.hpp"

static void	set_success_status(std::string& response,
					const std::string& value, int http_status)
{
	if (http_status == 200)
		response.append(" OK\r\n");
	else if (http_status == 201)
		response.append(" Created\r\n");
	else
		response.append(" Kamoulox\r\n");
	(void)value;
}

static void	set_redirect_status(std::string& response,
					const std::string& value, int http_status)
{
	if (http_status == 301)
		response.append(" Moved Permanently\r\n");
	else if (http_status == 302)
		response.append(" Found\r\n");
	else if (http_status == 303)
		response.append(" See Other\r\n");
	else if (http_status == 307)
		response.append(" Temporary Redirect\r\n");
	else if (http_status == 308)
		response.append(" Permanent Redirect\r\n");
	else
		response.append(" Kamoulox\r\n");
	if (!value.empty())
		response.append("Location:" + value + "\r\n");
}

int	http_response(Client& client, const std::string& value,
					int http_status, int ret)
{
	std::string	*response = new std::string;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	if (http_status >= 200 && http_status < 300)
		set_success_status(*response, value, http_status);
	else if (http_status >= 300 && http_status < 400)
		set_redirect_status(*response, value, http_status);
	if (!value.empty() && http_status < 300)
	{
		response->append("Content-Length:" + ft_int_to_string(value.size())
				+ "\r\n\r\n");
		response->append(value);
	}
	else
		response->append("Content-Length:0\r\n\r\n");
	client.setResponse(response);
	client.clearRequest();
	return ret;
}

int	http_response(Client& client, const std::string& value,
					int http_status, int ret, const std::string& filename,
					size_t filesize)
{
	std::string	*response = new std::string;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	if (http_status >= 200 && http_status < 300)
		set_success_status(*response, value, http_status);
	else if (http_status >= 300 && http_status < 400)
		set_redirect_status(*response, value, http_status);
	if (filename.empty())
		response->append("Content-Length:0\r\n\r\n");
	else
	{
		response->append("Content-Length:" +
				ft_int_to_string(filesize) + "\r\n\r\n");
		file_to_string(*response, filename, filesize);
	}
	client.setResponse(response);
	client.clearRequest();
	return ret;
}
