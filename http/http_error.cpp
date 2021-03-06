#include "webserv.hpp"

static void	set_default_err_page(std::string& response)
{
	std::string	err_page("<html>\n<head>\n<meta charset=\"utf-8\">\n"
					"<title>Webserv error page</title>\n</head>\n<body>"
					"\n<h2>Webserv - HTTP error:</h2>\n<p>");
	err_page.append(response);
	err_page.append("</p></body></html>");
	response.append("Content-Length:" + ft_int_to_string(err_page.size()) +
			"\r\n\r\n");
	response.append(err_page);
}

static void	set_err_status(std::string& response, const std::string& value,
				int http_status)
{
	if (http_status == 404)
		response.append(" Not Found\r\n");
	else if (http_status == 405)
	{
		response.append(" Method Not Allowed\r\n");
		if (!value.empty())
			response.append("Allow: " + value + "\r\n");
	}
	else if (http_status == 500)
		response.append(" Internal Server Error\r\n");
	else if (http_status == 415)
		response.append(" Unsupported Media Type\r\n");
	else if (http_status == 400)
		response.append(" Bad Request\r\n");
	else if (http_status == 413)
		response.append(" Payload Too Large\r\n");
	else if (http_status == 431)
		response.append(" Request Header Fields Too Large\r\n");
	else
		response.append(" Kamoulox\r\n");
}

//HTTP error response formatting.
//First creates the right status line according to http_status, then add if it
//exists an error page relative to configuration file into the body.
//Content-Length header is set accordingly.
int	http_error(Client& client, const std::map<int, std::string>& err,
					int http_status, int ret)
{
	std::map<int, std::string>::const_iterator	it;
	std::string	absolute_filename;
	std::string	*response = new std::string;
	int			file_size;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	set_err_status(*response, "", http_status);
	it = err.find(http_status);
	if (it != err.end())
	{
		absolute_filename.append(it->second);
		if (is_file_exist(absolute_filename))
		{
			file_size = get_file_size(absolute_filename.c_str());
			response->append("Content-Length:" +
							ft_int_to_string(file_size) + "\r\n\r\n");
			file_to_string(*response, absolute_filename, file_size);
		}
		else
			set_default_err_page(*response);
	}
	else
		set_default_err_page(*response);
	client.setResponse(response);
	client.clearRequest();
	return ret;
}

int	http_error(Client& client, const std::map<int, std::string>& err,
					const std::string& value, int http_status, int ret)
{
	std::map<int, std::string>::const_iterator	it;
	std::string	absolute_filename;
	std::string	*response = new std::string;
	int			file_size;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	set_err_status(*response, value, http_status);
	it = err.find(http_status);
	if (it != err.end())
	{
		absolute_filename.assign(it->second);
		if (is_file_exist(absolute_filename))
		{
			file_size = get_file_size(absolute_filename.c_str());
			response->append("Content-Length:" +
							ft_int_to_string(file_size) + "\r\n\r\n");
			file_to_string(*response, absolute_filename, file_size);
		}
		else
		set_default_err_page(*response);
	}
	else
		set_default_err_page(*response);
	client.setResponse(response);
	client.clearRequest();
	return ret;
}

int	http_error(Client& client, int http_status, int ret)
{
	std::string	*response = new std::string;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	set_err_status(*response, "", http_status);
	set_default_err_page(*response);
	client.setResponse(response);
	client.clearRequest();
	return ret;
}
