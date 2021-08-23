#include "processing.hpp"
#include <iostream>
#include <string>

std::string const *process_request(std::string& raw_request,
	const std::vector<Server>& server_blocks,
	const std::pair<std::string, int>& listen, int *flag)
{
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
}
