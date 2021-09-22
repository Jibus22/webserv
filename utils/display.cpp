#include "webserv.hpp"

const std::string	display_headers(const std::string& response)
{
	size_t	blankline = response.find("\r\n\r\n");

	if (blankline == std::string::npos)
	{
		blankline = response.find("\n\n");
		if (blankline == std::string::npos || blankline > 2000)
			return "";
	}
	return response.substr(0, blankline);
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
