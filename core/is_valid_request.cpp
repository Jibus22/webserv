#include "webserv.hpp"

std::string		*check_transfer_encoding(const std::string& request,
						size_t value_start, size_t body_pos, int& status)
{
	if (find_nocase_header(request, "Content-Length") != std::string::npos)
		return new std::string("HTTP/1.1 400 Bad Request");
	(void)value_start;
	(void)body_pos;
	(void)status;
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
		return new std::string("HTTP/1.1 400 Bad Request");
	value_end = request.find_first_of('\n', value_start);
	body_len = request.size() - body_pos;
	content_length = ft_string_to_nb(request.substr(value_start,
						(value_end - value_start)));
	if (content_length == INT_MAX)
		return new std::string("HTTP/1.1 400 Bad Request");
	if (body_len < content_length)
		return NULL;
	else if (body_len > content_length)
		return new std::string("HTTP/1.1 400 Bad Request");
	status = VALID_REQUEST;
	return NULL;
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
			return new std::string("HTTP/1.1 431 Request Header Fields Too Large");
		else
			return NULL;
	}
	if (blankline <= 14)// == request line mauvaise
		return new std::string("HTTP/1.1 400 Bad Request");
	hdr_pos = find_nocase_header(request, "Content-Length:");
	if (hdr_pos < blankline)
		return check_content_length(request,
				request.find_first_of(':', hdr_pos) + 1, blankline + 4, status);
	hdr_pos = request.find("Transfer-Encoding:");
	if (hdr_pos < blankline)
		return check_transfer_encoding(request,
				request.find_first_of(':', hdr_pos) + 1, blankline + 4, status);
	if (request.size() > blankline + 4)
		return new std::string("HTTP/1.1 400 Bad Request");
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
