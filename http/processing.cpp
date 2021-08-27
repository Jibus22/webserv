#include "processing.hpp"
#include <iostream>
#include <fstream>
#include <string>

const char* NoServerMatchException::what() const throw()
{
	return "There is no server match";
}

bool	match_server_name(Server *server, Request & request)
{
	(void)server;
	(void)request;
	return true;
}

//TODO: gere le 0.0.0.0
Serveur * match_server(std::vector<Serveur *> server_blocks,
					std::pair<std::string, int> listen, Request & requete)
{
	(void)requete;

	std::vector<Serveur *> matching_listen;
	for (std::vector<Serveur *>::iterator it = server_blocks.begin();
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

void	construct_response(Response & response, Serveur * server)
{
	//verifier les parametres de server

	//find matching location


	(void)response;
	(void)server;
}

void	process_request(Client& client,
				const std::vector<Serveur>& server_blocks)
{
	(void)client;
	(void)server_blocks;
}

void	process_request(Client& client,
				const std::vector<Serveur *>& server_blocks)
{

	try {
		Request r = Request(client.getRaw());

		// On recupere le serveur associe a la requete
		Serveur * s = match_server(server_blocks, client.getListen(), r);

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
