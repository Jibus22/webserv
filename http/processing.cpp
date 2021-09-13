#include "processing.hpp"
#include <iostream>
#include <fstream>
#include <string>

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
	//on recupere une liste avec une tres grande specificite de la correspondance
	for (std::vector<Server_config *>::iterator it = server_blocks.begin();
		it != server_blocks.end(); it++)
	{
		if((*it)->listen == listen)
			matching_listen.push_back(*it);
	}
	//si un seul match il repond
	if (matching_listen.size() == 1)
		return matching_listen.front();
	//si plus de 1 match on regarde le header host
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
	else
	//si 0 match
	{
		return matching_listen.front();
	}
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

void	error_page(int erreur, Response & response,
		Config_struct::c_error_map & error_page)
{
	if (error_page[erreur] != "")
	{
		std::string content;
		if (get_file_content(error_page[erreur], content))
			response.set_body(content);
	}
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
		if((target.compare(0, location->uri.size(), location->uri)) == 0)
			return location;
		it++;
	}
	return NULL;
}

bool	is_methode_allowed(Location_config * location, std::string methode)
{
	if (location == NULL && methode == "GET")
		return true;
	else if (location == NULL)
		return false;
	else
	{
		Config_struct::c_methode_vector::iterator it = location->methode.begin();
		while (it != location->methode.end())
		{
			if (*it == methode)
				return true;
			it++;
		}
		return false;
	}
}

bool	check_cgi(Response & response, Request & requete,
			Server_config * server, Location_config * location)
{
	if (location == NULL)
		return false;
	Config_struct::c_cgi_map::iterator it = location->cgi.begin();
	while (it != location->cgi.end())
	{
		if (requete.get_target().find(it->first) != std::string::npos)
		{
			process_cgi(response, requete, server, location, it->first);
			return true;
		}
		it++;
	}
	return false;
}



#include <iostream>
#include <vector>
#include <dirent.h>

bool is_dir(const std::string path)
{
	DIR *dir;

    if ((dir = opendir(path.c_str() + 1)) != nullptr) {
        closedir (dir);
		return 1;
    }
	else
		return false;
}

void	handle_index(std::string & target,
			const Config_struct::c_index_vector & index)
{
	std::ifstream		file;
/*
	file.open(target.c_str() + 1);
	if (file.fail() == false)
		return;
*/
	__D_DISPLAY("DIRECTORY");
	Config_struct::c_index_vector::const_iterator it = index.begin();
	std::string path;
	while (it != index.end())
	{
		path = target;
		path.append((*it));
		file.open(path.c_str() + 1);
		if (file.fail() == false)
		{
			target = path;
			return;
		}
		it++;
	}
}



void	construct_get_response(Response & response, Request &requete,
						Server_config * server, Location_config * location)
{
	std::string content;

	if (is_dir(requete.get_target()))
		handle_index(requete.get_target(), location->index);
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
		error_page(404, response, server->error_page);
	}
}

void	construct_post_response(Response & response, Request &requete)
{
	(void)response;
	(void)requete;
}

void	construct_delete_response(Response & response, Request &requete)
{
	(void)response;
	(void)requete;
}

void	construct_response(Response & response, Server_config * server,
														Request & requete)
{
	//TODO: verifier les parametres de server
	//Verification Body not too big
	std::stringstream str(requete["Content-Length"]);
	unsigned long length;
	str >> length;
	if (length > server->m_body_size)
	{
		response.set_status_code("413");
		response.set_status_infos("Payload Too Large");
		error_page(413, response, server->error_page);
	}

	//find matching location
	Location_config * location = match_location(server->location,
												requete.get_target());
	if (location)
	{__D_DISPLAY("location matched : " << location->uri);}

	if (check_cgi(response, requete, server, location))
		return;
	//check la conf location
	if (requete.get_method() == "GET" && is_methode_allowed(location, "GET"))
		construct_get_response(response, requete, server, location);
	else if (requete.get_method() == "POST" &&
			is_methode_allowed(location, "POST"))
		construct_post_response(response, requete);
	else if(requete.get_method() == "DELETE" &&
			is_methode_allowed(location, "DELETE"))
		construct_delete_response(response, requete);
	else
	{
		response.set_status_code("405");
		response.set_status_infos("Method Not Allowed");
		error_page(405, response, server->error_page);
	}
}

void	process_request(Client& client,
				const std::vector<Server_config *>& server_blocks)
{
	Response response;
	try {
		__D_DISPLAY("request : ");
		__D_DISPLAY(client.getStrRequest());
		Request r = Request(client.getStrRequest());
		__D_DISPLAY("Object Request Created");

		// On recupere le serveur associe a la requete
		Server_config * s = match_server(server_blocks, client.getListen(), r);
		__D_DISPLAY("server find");

		//Construction reponse
		construct_response(response, s, r);
		//__D_DISPLAY("response :")
		//__D_DISPLAY(*(response.get_raw()));
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
		response.set_status_code("400");
		response.set_status_infos("Bad Request");
	}
	client.setResponse(response.get_raw());
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
