#include "processing.hpp"

//return true si le server name match le header host de la requete
bool	match_server_name(Server_config *server, Request & request)
{
	Config_struct::c_name_vector::iterator it = server->name_serv.begin();
	while (it != server->name_serv.end())
		if (*it == request["Host"])
			return true;
	return false;
}

//TODO: gere le 0.0.0.0
//Renvoie un pointeur sur le server qui est associe a la requete
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

// Trouve la location associé a l'URI de la requete
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
		if (location->uri == "/")
		{
			__D_DISPLAY("location matched : " << location->uri);
			return location;
		}
		else if (location->uri[location->uri.size() - 1] == '/')
		{
			// cas ou location uri fini par /

			//on compare jusque avant le / de la location URI
			//ca match si compare == 0 et si soit la target est fini apres la comparaison
			// soit si il y a un / apres
			//Par exemple /dir/
			//doit matcher avec /dir /dir/ /dir/test.html
			// mais pas /directory
			if((target.compare(0, location->uri.size() - 1,
			location->uri, 0, location->uri.size() - 1) == 0)
			&& (target.size() == location->uri.size() - 1 ||
				target[location->uri.size() - 1] == '/'))
			{
				__D_DISPLAY("location matched : " << location->uri);
				return location;
			}
		}
		else
		{
			//cas ou location fini pas par /

			//pareil mais longueur de comparaison change
			if((target.compare(0, location->uri.size(), location->uri) == 0) &&
(target.size() == location->uri.size() || target[location->uri.size()] == '/'))
			{
				__D_DISPLAY("location matched : " << location->uri);
				return location;
			}
		}
		it++;
	}
	return NULL;
}

//Ajoute dans la reponse la page d'erreur associe a l'erreur defini dans la conf
void	error_page(int erreur, Response & response,
		Config_struct::c_error_map & error_page)
{
	if (error_page[erreur] != "")
	{
		std::string content;
		if (get_file_content(error_page[erreur], content))
			response.set_body(content);
		std::stringstream ss;
		ss << content.size();
		response.add_header("Content-Length", ss.str());
	}
	else
		response.add_header("Content-Length", "0");
}

//renvoie true si methode autorisé false sinon
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
	size_t pos;
	while (it != location.cgi.end())
	{
		if ((pos = requete.get_target().find(it->first)) != std::string::npos &&
	(	requete.get_target().size() == pos + it->first.size() ||
		requete.get_target()[pos + it->first.size()] == '/'
	)
		)
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

//Met a jour la target avec la directive root
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

//si la target est un directory teste si le fichier existe dans le dossier
//et met a jour la requete avec le fichier qui doit etre renvoye
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
	//std::string path;
	while (it != location->index.end())
	{
		if (is_file_exist(target + *it))
		{
			target.append(*it);
			return;
		}
			/*
		path = target;
		path.append((*it));
		file.open(path.c_str());
		if (file.fail() == false)
		{
			target = path;
			file.close();
			return;
		}
		file.close();
		*/
		it++;
	}
}


//Si une redirection doit etre fait met le code et le status dans la reponse
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
	response.add_header("Content-Length", "0");
}

void	construct_get_response(Response & response, Request &requete,
						Server_config * server, Location_config * location)
{
	std::string content;

	//si la target est un repertoire cherche le fichier a repondre
	if (is_dir(requete.get_target()))
		handle_index(requete.get_target(), location);

	// si la target est un repertoire et que l'autoindex est active
	//renvoie l'autoindex
	//sinon si la target est un fichier et qu'il peut etre recupéré
	//set la reponse a 200 et renvoie le fichier
	//sinon
	//renvoie une erreur car fichier untrouvable
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
		__D_DISPLAY("content : "<< ss.str());
		response.add_header("Content-Length", ss.str());
	}
	else
	{
		response.set_status_code("404");
		response.set_status_infos("Not Found");
		error_page(404, response, server->error_page);
	}
}

int	http_post(Request& request)
{
	std::string	value;

	if (request.getHeader("content-type", value) == false)
		return 1;
	if (value.find("multipart/form-data") != std::string::npos)
		return 1;
		//return formdata_process(request, value);

	return 1;
}

void	construct_delete_response(Response & response, Request &requete)
{
	(void)response;
	(void)requete;
}

//set le code 405 rt le message associe et les headers
//Header allow qui donne les methodes autorisé pour cette ressource
void	method_not_allowed(Response & response, Server_config * server,
		Location_config * location)
{
	std::string  str;
	bool get = false;
	bool post = false;
	bool del = false;

	response.set_status_code("405");
	response.set_status_infos("Method Not Allowed");
	Config_struct::c_methode_vector::iterator it = location->methode.begin();
	Config_struct::c_methode_vector::iterator ite = location->methode.end();
	while (it != ite)
	{
		if (*it == "GET")
			get = true;
		else if (*it == "POST")
			post = true;
		else if (*it == "DELETE")
			del = true;
		it++;
	}
	if (get)
		str.append("GET");
	if (get && (post || del))
		str.append(", ");
	if (post)
		str.append("POST");
	if (post && del)
		str.append(", ");
	if (del)
		str.append("DELETE");
	response.add_header("Allow", str);
	response.add_header("Content-Length", "0");
	error_page(405, response, server->error_page);
}

int		construct_response(Response & response, const Server_config * server,
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

	if (!location)
	{
		response.set_status_code("404");
		response.set_status_infos("Not Found");
		error_page(404, response, server->error_page);
		return 0;
	}
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
		return http_post(requete);
	else if(requete.get_method() == "DELETE" &&
			is_methode_allowed(location, "DELETE"))
		construct_delete_response(response, requete);
	else
		method_not_allowed(response, server, location);
	return 0;
}

void	process_request(Client& client,
				const std::vector<Server_config *>& server_blocks,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	size_t	len_request = client.getRequestSize();//TMP!
	Response response;

	try {
		__D_DISPLAY("request : ");
		__D_DISPLAY(client.getStrRequest());
		Request r(client.getStrRequest());
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
		response.add_header("Content-Length", "0");
	}
	client.setResponse(response.get_raw());
	client.truncateRequest(len_request);
}
