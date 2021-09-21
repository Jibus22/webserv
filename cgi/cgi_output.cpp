#include "webserv.hpp"

bool	is_status_line(const std::string& cgi_out)
{
	size_t	pos = cgi_out.find("HTTP/1.");

	if (pos > 0)
		return false;
	pos += 8;
	if (pos > cgi_out.size() || cgi_out[pos] != ' ')
		return false;
	pos++;
	for (int i = 0; i < 3; i++)
	{
		if (pos > cgi_out.size())
			return false;
		if (!isdigit(cgi_out[pos]))
			return false;
		pos++;
	}
	if (pos > cgi_out.size() || !isspace(cgi_out[pos]))
		return false;
	return true;
}

bool	is_relative_uri(size_t val, const std::string& headers)
{
	while (val < headers.size() && headers[val] == ' ')
		val++;
	if (headers[val] != '/')
		return false;
	return true;
}

//CGI response format: header - blankline - body
//If header = content-type -> document response -> check/set status & check/set
//							'content-length' header
//If header = Location -> 
//					- If relative URI -> local redirecttion -> redirect stat
//					- If absolute URI -> client redirection -> set response.
int		cgi_output(std::string& cgi_out)
{
	size_t		val, end, blankline = cgi_out.find("\r\n\r\n");
	std::string	headers, cont_len("");

	if (blankline == std::string::npos)
		return CGI_ERR;//err 500
	headers = cgi_out.substr(0, blankline);
	if (find_nocase_header(headers, "Content-Type:") != std::string::npos)
	{
		if (find_nocase_header(headers, "Content-Length:") == std::string::npos)
			cont_len = "Content-Length:" +
				ft_int_to_string(cgi_out.size() - (blankline + 4)) + "\r\n";
		if (!is_status_line(cgi_out))
			cgi_out.insert(0, "HTTP/1.1 200 OK\r\n" + cont_len);
		else if (!cont_len.empty())
			cgi_out.insert(blankline + 3, cont_len);
		return CGI_SUCCESS;
	}
	else if ((val = find_nocase_header(headers, "Location:"))
					!= std::string::npos)
	{
		val += 9;
		if (is_relative_uri(val, headers))
		{
			end = headers.find_first_of('\n', val);
			cgi_out.assign(headers.substr(val, end - val));
			return CGI_REDIRECT;
		}
	}
	return CGI_SUCCESS;
}
