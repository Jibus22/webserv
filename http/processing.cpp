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
	Config_struct::c_name_vector::iterator it = server->name_serv.begin();
	while (it != server->name_serv.end())
		if (*it == request["Host"])
			return true;
	return false;
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
		for (std::vector<Server_config *>::iterator it = matching_listen.begin();
			it != matching_listen.end(); it++)
		{
			if (match_server_name(*it, requete))
				return *it;
		}
		return matching_listen.front();
	}
	throw NoServerMatchException();
}


bool	get_file_content(std::string const & path, std::string & content)
{
	std::ifstream		file;
	std::string			line;

	file.open(path.c_str());
	if (file.fail() == true)
		return false;
	while (std::getline(file, line))
		content.append(line);
	return true;
}

Location_config * match_location(Config_struct::c_loc_map & map_location,
											std::string target)
{
	if (map_location.empty())
		return NULL;
	Config_struct::c_loc_map::iterator it = map_location.begin();
	while (it != map_location.end())
	{
		if(it->first.compare(0, it->first.size(), target) == 0)
			return it->second;
	}
	return NULL;
}

void	construct_response(Response & response, Server_config * server,
														Request & requete)
{
	//TODO: verifier les parametres de server
	// ...

	//find matching location
	Location_config * location = match_location(server->location,
												requete.get_target());
	//check la conf location
	if (requete.get_method() == "GET")
	{
		std::string content;
		if (get_file_content(requete.get_target(), content))
		{
			response.set_status_code(200);
			response.set_status_infos("OK");
			response.set_body(content);
		}
		else
		{
			response.set_status_code(404);
			response.set_status_infos("Not Found");
		}
	}
	//else
	//

	(void)response;
	(void)location;
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
		construct_response(response, s, r);

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
		//Requete non termine pas de set reponse
		return;
	}
	catch (Request::InvalidRequest e)
	{
		//Requete non valide renvoyer reponse avec code erreur non valide
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
