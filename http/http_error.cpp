#include "webserv.hpp"

static void	set_err_status(std::string& response, int http_status)
{
	if (http_status == 404)
		response.append(" Not Found\r\n");
	else if (http_status == 500)
		response.append(" Internal Server Error\r\n");
	else if (http_status == 415)
		response.append(" Unsupported Media Type\r\n");
	else if (http_status == 400)
		response.append(" Bad Request\r\n");
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
	set_err_status(*response, http_status);
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
			response->append("Content-Length:0\r\n\r\n");
	}
	else
		response->append("Content-Length:0\r\n\r\n");
	client.setResponse(response);
	return ret;
}
