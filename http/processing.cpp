#include "processing.hpp"
#include <iostream>
#include <fstream>
#include <string>

const char* NoServerMatchException::what() const throw()
{
	return "There is no server match";
}

bool	match_server_name(Server_config *server, Request & request)
{
	(void)server;
	(void)request;
	return true;
}

//TODO: gere le 0.0.0.0
Server_config * match_server(std::vector<Server_config *> server_blocks,
					std::pair<std::string, int> listen, Request & requete)
{
	(void)requete;

	std::vector<Server_config *> matching_listen;
	for (std::vector<Server_config *>::iterator it = server_blocks.begin();
		it != server_blocks.end(); it++)
	{
		if((*it)->listen == listen)
			matching_listen.push_back(*it);
	}
	if (matching_listen.size() == 1)
		return matching_listen.front();
	else if (matching_listen.size() > 1)
	{

	}
	throw NoServerMatchException();
}

/*
std::ifstream get_file_content(std::string & path)
{
	std::ifstream		file;
	file.open(path.c_str());

	if (file.fail() == true)
		throw std::runtime_error("Open file");
	return file;
}*/

void	construct_response(Response & response, Server_config * server)
{
	//verifier les parametres de server

	//find matching location


	(void)response;
	(void)server;
}

void	process_request(Client& client,
				const std::vector<Server_config>& server_blocks)
{
	(void)client;
	(void)server_blocks;
}

void	process_request(Client& client,
				const std::vector<Server_config *>& server_blocks)
{

	try {
		Request r = Request(client.getStrRequest());

		// On recupere le serveur associe a la requete
		Server_config * s = match_server(server_blocks, client.getListen(), r);

		Response response;

		//Construction reponse
		construct_response(response, s);

		//client.setResponse(response.get_raw(), COMPLETE);
		client.setResponse(response.get_raw());
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
		//client.setFlag(INCOMPLETE);
		return;
	}
	catch (Request::InvalidRequest e)
	{
		//Requete non valide renvoyer reponse non valide
		//TODO: Reponse invlaide
		return;
	}
}

/*
void	process_request(Client& client,
				const std::vector<Server_config*>& server_blocks)
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

			//--------------//
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
*/
