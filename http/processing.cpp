#include "processing.hpp"
#include <iostream>
#include <string>

const char* NoServerMatchException::what() const throw()
{
	return "There is no server match";
}

SiServ & match_server(std::vector<SiServ> server_blocks,
					std::pair<std::string, int> listen)
{
	for (std::vector<SiServ>::iterator it = server_blocks.begin();
	it != server_blocks.end(); it++)
	{
		if(it->listen == listen)
			return *it;
	}
	throw NoServerMatchException();
}

void	construct_response(Response & response, SiServ & server)
{
	(void)response;
	(void)server;
}

void	process_request(Client& client,
				const std::vector<SiServ>& server_blocks)
{

	try {
		// On recupere le serveur associe a la requete
		SiServ & s = match_server(server_blocks, client.getListen());

		Request r = Request(client.getRaw());

		Response response;

		//Construction reponse
		construct_response(response, s);

		client.setResponse(response.get_raw(), COMPLETE);
		return;
	}
	catch (NoServerMatchException e)
	{
		//TODO: Pas de match
		return;
	}
	catch (Request::NotTerminatedException e)
	{
		//Requete non termine set flag
		client.setFlag(INCOMPLETE);
		return;
	}
	catch (Request::InvalidRequest e)
	{
		//Requete non valide renvoyer reponse non valide
		//TODO: Reponse invlaide
		return;
	}
}
