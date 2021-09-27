#include "webserv.hpp"

//Check the end of the chunked request which must be '0\r\n\r\n'
//If the request is bigger than 1MB, returns error.
std::string		*check_transfer_encoding(const std::string& request,
						size_t value_start, size_t body_pos, int& status)
{
	size_t	chunked_end, value_end;

	if (find_nocase_header(request, "Content-Length") != std::string::npos)
		return new std::string("HTTP/1.1 400 Bad Request\r\n");
	value_end = request.find_first_of('\n', value_start);
	if (find_nocase_header(request.substr(value_start,
					(value_end - value_start)), "chunked") != std::string::npos)
	{
		chunked_end = request.find("0\r\n\r\n", body_pos);
		if (chunked_end == std::string::npos)
			return NULL;
	}
	status = VALID_REQUEST;
	return NULL;
}

//Check if the content_length header value is valid & match with the body length
std::string		*check_content_length(const std::string& request,
						size_t value_start, size_t body_pos, int& status)
{
	size_t		value_end, body_len;
	size_t		content_length;

	if (find_nocase_header(request, "Transfer-Encoding") != std::string::npos)
		return new std::string("HTTP/1.1 400 Bad Request\r\n");
	value_end = request.find_first_of('\n', value_start);
	body_len = request.size() - body_pos;
	content_length = ft_string_to_nb(request.substr(value_start,
						(value_end - value_start)));
	if (content_length == INT_MAX)
		return new std::string("HTTP/1.1 400 Bad Request\r\n");
	if (body_len < content_length)
		return NULL;
	else if (body_len > content_length)
		return new std::string("HTTP/1.1 400 Bad Request\r\n");
	status = VALID_REQUEST;
	return NULL;
}

//Check syntax of request line and 'Host:' header
int				is_basics(const std::string& request, size_t blankline)
{
	size_t		pos, space = 0, end_request_line = request.find_first_of('\n');

	if (blankline < 24)//Minimal length of request line + Host header
		return 0;
	for (size_t i = 0; i < end_request_line; i++)
	{
		if (request[i] == ' ')
			space++;
		if (space == 0)
		{
			if (!isupper(request[i]))
				return 0;
		}
		else if (space == 1)
		{
			if (i < 3 || request[i + 1] != '/')
				return 0;
			space = 41;
		}
		else if (space == 42)
		{
			pos = request.find("HTTP/1.1", i + 1);
			if (pos == std::string::npos || pos > i + 1)
				return 0;
			break;
		}
	}
	pos = find_nocase_header(request, "Host:");
	if (pos == std::string::npos || pos > blankline)
		return 0;
	return 42;
}

//look for blank line, check if request line can exists & if headers don't size
//more than 2KB.
//Then check if there is Content-Length or Transfer-Encoding headers.
//They each have they own behavior & meaning so there function returns
//appropriate exit code.
//if there is no such headers there is no body, or this is an error;
std::string		*parse_request(const Client& client, int& status)
{
	const std::string&	request = client.getStrRequest();
	size_t				hdr_pos, blankline = request.find("\r\n\r\n");

	if (blankline == std::string::npos)
	{
		if (request.size() > 2048)//2KB of headers without blank line = error
			return new std::string("HTTP/1.1 431 Request Header"
					" Fields Too Large\r\n");
		else
			return NULL;
	}
	if (!is_basics(request, blankline))
		return new std::string("HTTP/1.1 400 Bad Request\r\n");
	hdr_pos = find_nocase_header(request, "Content-Length:");
	if (hdr_pos < blankline)
		return check_content_length(request,
				request.find_first_of(':', hdr_pos) + 1, blankline + 4, status);
	hdr_pos = request.find("Transfer-Encoding:");
	if (hdr_pos < blankline)
		return check_transfer_encoding(request,
				request.find_first_of(':', hdr_pos) + 1, blankline + 4, status);
	if (request.size() > blankline + 4)
		return new std::string("HTTP/1.1 400 Bad Request\r\n");
	status = VALID_REQUEST;
	return NULL;
}

int		is_valid_request(Client& client)
{
	int			status = INCOMPLETE_REQUEST;
	std::string	*error = parse_request(client, status);

	//__D_DISPLAY("CLIENT -_-_-_-_-_-_-_-:\n" << client);
	if (error)
	{
		__D_DISPLAY("Corrupt request: " << *error);
		status = CORRUPT_REQUEST;
		client.truncateRequest((client.getStrRequest()).size());
		client.setResponse(error);
	}
	__D_DISPLAY("is_valid_request status (VALID=0, INCOMPLETE=1): " << status);
	return status;
}
