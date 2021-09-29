#include "webserv.hpp"

static void	set_basic_hdr(std::string& response)
{
	response.append("Content-Length:0\r\n\r\n");
}

static void	file_to_string(std::string& response,
				const std::string& filename, int filesize)
{
	char	*buf = new char [filesize];
	int		fd = open(filename.c_str(), O_RDONLY);

	if (fd == -1)
		return ;
	read(fd, buf, filesize);
	response.append(buf);
	delete [] buf;
}

int	http_error(Client& client, const std::map<int, std::string>& err,
					int http_status, int ret)
{
	std::map<int, std::string>::const_iterator	it;
	std::string	absolute_filename;
	std::string	*response = new std::string;
	int			file_size;

	response->assign("HTTP/1.1 ");
	response->append(ft_int_to_string(http_status));
	if (http_status == 404)
		response->append(" Not Found\r\n");
	else if (http_status == 500)
		response->append(" Internal Server Error\r\n");
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
			set_basic_hdr(*response);
	}
	else
		set_basic_hdr(*response);
	client.setResponse(response);
	return ret;
}
