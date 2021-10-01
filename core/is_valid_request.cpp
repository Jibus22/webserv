#include "webserv.hpp"

//Check the end of the chunked request which must be '0\r\n\r\n'
//If the request is bigger than 1MB, returns error.
static int	check_transfer_encoding(Client& client, const std::string& request,
						size_t value_start, size_t body_pos)
{
	size_t	chunked_end, value_end;

	if (find_nocase_header(request, "Content-Length") != std::string::npos)
		return http_error(client, 400, CORRUPT_REQUEST);
	value_end = request.find_first_of('\n', value_start);
	if (find_nocase_header(request.substr(value_start,
					(value_end - value_start)), "chunked") != std::string::npos)
	{
		chunked_end = request.find("0\r\n\r\n", body_pos);
		if (chunked_end == std::string::npos)
			return INCOMPLETE_REQUEST;
	}
	return VALID_REQUEST;
}

//Check if the content_length header value is valid & match with the body length
static int	check_content_length(Client& client, const std::string& request,
						size_t value_start, size_t body_pos)
{
	size_t	value_end, body_len;
	size_t	content_length;

	if (find_nocase_header(request, "Transfer-Encoding") != std::string::npos)
		return http_error(client, 400, CORRUPT_REQUEST);
	value_end = request.find_first_of('\n', value_start);
	body_len = request.size() - body_pos;
	content_length = ft_string_to_nb(request.substr(value_start,
						(value_end - value_start)));
	if (content_length == INT_MAX)
		return http_error(client, 400, CORRUPT_REQUEST);
	if (body_len < content_length)
		return INCOMPLETE_REQUEST;
	else if (body_len > content_length)
		return http_error(client, 400, CORRUPT_REQUEST);
	return VALID_REQUEST;
}

//Check syntax of request line and 'Host:' header
static int	is_basics(const std::string& request, size_t blankline)
{
	size_t	pos, space = 0, end_request_line = request.find_first_of('\n');

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
static int	parse_request(Client& client)
{
	const std::string&	request = client.getStrRequest();
	size_t				hdr_pos, blankline = request.find("\r\n\r\n");

	if (blankline == std::string::npos)
	{
		if (request.size() > 2048)//2KB of headers without blank line = error
			return http_error(client, 431, CORRUPT_REQUEST);
		else
			return INCOMPLETE_REQUEST;
	}
	if (!is_basics(request, blankline))
		return http_error(client, 400, CORRUPT_REQUEST);
	hdr_pos = find_nocase_header(request, "Content-Length:");
	if (hdr_pos < blankline)
		return check_content_length(client, request,
				request.find_first_of(':', hdr_pos) + 1, blankline + 4);
	hdr_pos = request.find("Transfer-Encoding:");
	if (hdr_pos < blankline)
		return check_transfer_encoding(client, request,
				request.find_first_of(':', hdr_pos) + 1, blankline + 4);
	if (request.size() > blankline + 4)
		return http_error(client, 400, CORRUPT_REQUEST);
	return VALID_REQUEST;
}

int		is_valid_request(Client& client)
{
	int	status = parse_request(client);
	__D_DISPLAY("is_valid_request status (VALID=0, INCOMPLETE=1): " << status);
	display_request_hdr(client.getStrRequest(), status);
	return status;
}
