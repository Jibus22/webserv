#include "processing.hpp"
#include <iostream>
#include <string>

std::string const *process_request(std::string& raw_request,
	const std::vector<Server>& server_blocks,
	const std::pair<std::string, int>& listen, int *flag)
{
	/*
	try {
		Request r = Request(raw_request);

		// On recupere le serveur associe a la requete
		Server & s = match_server(server_blocks, listen, r);
		Response response;
		return response.get_raw();
	}
	catch (Request::NotTerminatedException e)
	{
		//Requete non termine set flag
		return NULL;
	}
	catch (Request::InvalidRequest e)
	{
		//Requete non valide set flag
		return NULL;
	}
	*/
	(void)raw_request;
	(void)server_blocks;
	(void)listen;
	(void)flag;
	return NULL;
}

void	process_request(Client& client,
				const std::vector<SiServ>& server_blocks)
{
	Response	test;
	Request		test2;
	const char	*request = client.getRawRequest();
	std::string	*response;

	__D_DISPLAY(client);

	for (int i = 0; i < client.getRequestSize(); i++)
	{
		if (*request == 'x')
		{
			response = new std::string(client.getRawRequest(), request + 1);
			response->insert(0, "Response: ");
			client.truncateRequest(request + 1);
			client.setResponse(response);
			__D_DISPLAY("X had been found into the request!");
			__D_DISPLAY("Truncated request: |" << client.getStrRequest() << "|");
			__D_DISPLAY("Response: |" << client.getStrResponse() << "|");
		}
		request++;
	}

	(void)server_blocks;
	(void)test;
	(void)test2;
}
