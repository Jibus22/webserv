#include "processing.hpp"

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
		if((target.compare(0, location->uri.size(), location->uri) == 0))// &&
//(target.size() == location->uri.size() || target[location->uri.size()] == '/'))
		{
			__D_DISPLAY("location matched : " << location->uri);
			return location;
		}
		it++;
	}
	return NULL;
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

int		check_cgi(Request& requete, const Server_config& server,
			const Location_config& location, Client& client,
			const std::map<int, Client>& client_map,
			const std::map<int, std::pair<std::string, int> >& server_map)
{
	int	ret = -1;
	Config_struct::c_cgi_map::const_iterator it = location.cgi.begin();

	while (it != location.cgi.end())
	{
		if (requete.get_target().find(it->first) != std::string::npos)
		{
			ret = process_cgi(requete, location,
					server, client, it->first, client_map, server_map);
			__D_DISPLAY("CGI status (0:SUCCESS - 1:REDIRECT - 2:ERROR): "
					<< ret);
			return ret;
		}
		it++;
	}
	return ret;
}

void	handle_root(std::string & target, Location_config * location)
{
	if (location == NULL || location->root == "")
		return;
	if (location->uri[location->uri.size()] == '/' || location->uri.size() == 1)
		target.erase(0 , location->uri.size());
	else
		target.erase(0 , location->uri.size() + 1);
	if (location->root[location->root.size() - 1] == '/')
		target.insert(0, location->root);
	else
		target.insert(0, location->root + "/");
}

void	handle_index(std::string & target, Location_config * location)
{
	std::ifstream		file;
/*
	file.open(target.c_str());
	if (file.fail() == false)
		return;
*/
	__D_DISPLAY("DIRECTORY");
	if (location == NULL)
		return;
	Config_struct::c_index_vector::const_iterator it = location->index.begin();
	std::string path;
	while (it != location->index.end())
	{
		path = target;
		path.append((*it));
		file.open(path.c_str());
		if (file.fail() == false)
		{
			target = path;
			return;
		}
		it++;
	}
}

void	handle_return(Response & response, Location_config *location)
{
	std::stringstream sstream;
    sstream << location->return_p.first;

	response.set_status_code(sstream.str());
	if (location->return_p.first == 301)
		response.set_status_infos("Moved Permanently");
	else if (location->return_p.first == 302)
		response.set_status_infos("Found");
	else if (location->return_p.first == 307)
		response.set_status_infos("Temporary Redirect");
	else if (location->return_p.first == 308)
		response.set_status_infos("Permanent Redirect");
	response.add_header("Location", location->return_p.second);
}

void	construct_get_response(Response & response, Request &requete,
						Server_config * server, Location_config * location)
{
	std::string content;

	if (is_dir(requete.get_target()))
		handle_index(requete.get_target(), location);
	if (is_dir(requete.get_target()) && location->auto_index == true)
		auto_index(response, requete.get_target());
	else if (!is_dir(requete.get_target()) &&
		get_file_content(requete.get_target(), content))
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

int		construct_response(Response & response, Server_config * server,
				Request & requete, Client& client,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{

	int					ret = 0;
	Location_config		*location;

	if (requete["Content-Length"] != "")
	{
		std::stringstream	str(requete["Content-Length"]);
		unsigned long		length;
		str >> length;
		if (length > server->m_body_size)
		{
			__D_DISPLAY("Payload too large");
			__D_DISPLAY("length" <<  requete["Content-Length"]);
			__D_DISPLAY("length" <<  length);
			__D_DISPLAY("m_body_size" <<  server->m_body_size);

			response.set_status_code("413");
			response.set_status_infos("Payload Too Large");
			error_page(413, response, server->error_page);
			return 0;
		}
	}

	location = match_location(server->location, requete.get_target());

	if (location)
		{__D_DISPLAY("code return : " << location->return_p.first);}
	if (location && location->return_p.first != 0)
	{
		handle_return(response, location);
		return 0;
	}

	//CGI
	while (location && (ret = check_cgi(requete, *server, *location,
					client, client_map, server_map)) >= 0)
	{
		if (ret == CGI_REDIRECT)
			location = match_location(server->location, requete.get_target());
		else
			return 1;
	}

	handle_root(requete.get_target(), location);

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
	return 0;
}

void	process_request(Client& client,
				const std::vector<Server_config *>& server_blocks,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	size_t	len_request = (client.getStrRequest()).size();//TMP!
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
		if (construct_response(response, s, r, client, client_map, server_map))
		{
			client.truncateRequest(len_request);
			return ;
		}
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
	client.truncateRequest(len_request);
}
