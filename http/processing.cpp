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
	__D_DISPLAY("target : " << path);
	std::ifstream		file;
	std::string			line;

	file.open(path.c_str() + 1);
	if (file.fail() == true)
		return false;
	while (std::getline(file, line))
		content.append(line + "\n");
	return true;
}

Location_config * match_location(Config_struct::c_location_vector & locations,
											std::string target)
{
	Location_config * location;

	if (locations.empty())
		return NULL;
	Config_struct::c_location_vector::iterator it = locations.begin();
	while (it != locations.end())
	{
		location = *it;
		if(location->uri.compare(0, location->uri.size(), target) == 0)
			return location;
		it++;
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
	__D_DISPLAY("location matched");
	//check la conf location
	if (requete.get_method() == "GET")
	{
		std::string content;
		if (get_file_content(requete.get_target(), content))
		{
			response.set_status_code("200");
			response.set_status_infos("OK");
			response.set_body(content);
			std::stringstream ss;
			ss << content.size();
			response.add_header("Content-Length", ss.str());
		}
		else
		{
			response.set_status_code("404");
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
		__D_DISPLAY("request created");

		// On recupere le serveur associe a la requete
		Server_config * s = match_server(server_blocks, client.getListen(), r);
		__D_DISPLAY("server find");

		Response response;

		//Construction reponse
		construct_response(response, s, r);
		__D_DISPLAY("response :")
		__D_DISPLAY(response.get_raw());
		//client.setResponse(response.get_raw(), COMPLETE);
		client.setResponse(response.get_raw());
		return;
	}
	catch (NoServerMatchException e)
	{
		//TODO: Pas de match
		__D_DISPLAY("No SERVER MATCH");
		return;
	}
	catch (Request::NotTerminatedException e)
	{
		//Requete non termine pas de set reponse
		__D_DISPLAY("NoT TERMINATED REQUEST");
		return;
	}
	catch (Request::InvalidRequest e)
	{
		//Requete non valide renvoyer reponse avec code erreur non valide
		//TODO: Reponse invlaide
		__D_DISPLAY("INVALID REQUEST");
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
