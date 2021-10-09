#include "webserv.hpp"

const std::string	display_headers(const std::string& response)
{
	size_t	blankline = response.find("\r\n\r\n");

	if (blankline == std::string::npos)
	{
		blankline = response.find("\n\n");
		if (blankline == std::string::npos || blankline > 3000)
		{
			__D_DISPLAY("display_headers() error: blankline not found");
			return "";
		}
	}
	return response.substr(0, blankline);
}

void	display_request_hdr(const std::string& request, int status)
{
	if (status == 0 || status == 2)
	{__D_DISPLAY(CYAN_C << display_headers(request) << RESET_C);}
}

void	__D_DISPLAY_RECV(int client_id, int len)
{
	__D_DISPLAY(GREEN_C << "Client{" << client_id
		<< "} - [RECV] - nb of bytes received :[" << len << "]" << RESET_C);
}

void	__D_DISPLAY_SEND(int client_id, int len,
						int send_id, const std::string& response)
{
	__D_DISPLAY(YELLOW2_C "Client{" << client_id
			<< "} - [SEND][" << send_id << "] - len of sent message :[" << len
			<< "]\nHeaders:\n" << display_headers(response)
			<< "\n[END SEND]" << RESET_C);
}

void	__DISPLAY_SERVERMAP(std::map<int, std::pair<std::string, int> >& srvmp)
{
	__D_DISPLAY("Display map of servers");
	for (std::map<int, std::pair<std::string, int> >::const_iterator i
			= srvmp.begin(); i != srvmp.end(); i++)
	{
		__D_DISPLAY("fd: " << i->first << " - ip: " << i->second.first
				<< " - port: " << i->second.second);
	}
}
