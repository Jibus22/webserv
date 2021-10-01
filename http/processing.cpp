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
	struct stat sb;
	if (error_page[erreur] != "" && is_openable(error_page[erreur]) &&
		stat(error_page[erreur].c_str(), &sb) != -1)
	{
		response.set_body_path(error_page[erreur]);
		std::stringstream ss;
		ss << sb.st_size;
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

//Look for cgi_ext directives into location block and its values, to compare
//with the target, to find cgi file extension
int		check_cgi(Request& request, const Server_config& server,
			const Location_config& location, Client& client,
			const std::map<int, Client>& client_map,
			const std::map<int, std::pair<std::string, int> >& server_map)
{
	Config_struct::c_cgi_map::const_iterator
						it = location.cgi.begin(),
						end = location.cgi.end();
	const std::string&	target = request.get_target();
	int					ret = -1;
	size_t				pos, len;

	while (it != end)
	{
		pos = target.find(it->first);
		if (pos != std::string::npos)
		{
			len = pos + it->first.size();
			if (len < target.size() &&
					(target[len] != '/' && target[len] != '?'))
			{
				it++;
				continue;
			}
			ret = process_cgi(request, location, server, client, it->first,
					client_map, server_map);
		__D_DISPLAY("CGI status (0:SUCCESS - 1:REDIRECT - 2:ERROR): " << ret);
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


/*
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
}*/

int		construct_get_response(Response & response, Request &requete,
						Server_config & server, Location_config * location,
						Client& client)
{
	size_t	filesize;

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
	else if (!is_dir(requete.get_target()) && is_openable(requete.get_target()))
	{
		filesize = get_file_size(requete.get_target().c_str());
		return http_response(client, "", 200, 1, requete.get_target(), filesize);
	}
	else
		return http_error(client, server.error_page, 404, 404);
	return 0;
}

int		http_post(Client& client, const Request& request,
				const Location_config& location, const Server_config& server)
{
	std::string	value;

	if (request.getHeader("content-type", value) == false
				|| location.upload_dir.empty())
		return http_error(client, server.error_page, 400, 1);
	if (value.find("multipart/form-data") != std::string::npos)
		return formdata_process(client, request.getRequest(), value,
						location.upload_dir, request.getBodyPos(), server,
						request.get_target());
	else
		return http_error(client, server.error_page, 415, 1);
}

int		http_delete(Client& client, const Request& request,
				const Server_config& server)
{
	if (is_dir(request.get_target()))
		return http_error(client, server.error_page, 400, 400);
	else if (!is_openable(request.get_target()))
		return http_error(client, server.error_page, 404, 404);
	if (unlink(request.get_target().c_str()) == -1)
		return http_error(client, server.error_page, 500, 500);
	else
		return http_response(client, "", 200, 200);
}

//set le code 405 rt le message associe et les headers
//Header allow qui donne les methodes autorisé pour cette ressource
void	method_not_allowed(Response & response, Server_config & server,
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
	error_page(405, response, server.error_page);
}

int		construct_response(Response& response, Server_config& server,
				Request& request, Client& client,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{

	int				ret = 0;
	Location_config	*location;

	location = match_location(server.location, request.get_target());
	if (!location)
		return http_error(client, server.error_page, 404, 1);
	if (location && location->return_p.first != 0)
		return http_response(client, location->return_p.second,
				location->return_p.first, location->return_p.first);
	//CGI
	while (ret != -1)
	{
		ret = check_cgi(request, server, *location, client,
				client_map, server_map);
		if (ret == -1)
			break ;
		else if (ret == CGI_REDIRECT)
		{
			location = match_location(server.location, request.get_target());
			if (!location)
				return http_error(client, server.error_page, 404, 1);
		}
		else
			return 1;
	}

	handle_root(request.get_target(), location);

	if (request.get_method() == "GET" && is_methode_allowed(location, "GET"))
		return construct_get_response(response, request, server, location, client);
	else if (request.get_method() == "POST" &&
			is_methode_allowed(location, "POST"))
		return http_post(client, request, *location, server);
	else if(request.get_method() == "DELETE" &&
			is_methode_allowed(location, "DELETE"))
		return http_delete(client, request, server);
	else
		method_not_allowed(response, server, location);
	return 0;
}

int		process_request(Client& client,
				const std::vector<Server_config *>& server_blocks,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	Server_config	*server;
	Response		response;
	Request 		request(client.getStrRequest());

	__D_DISPLAY(request);
	server = match_server(server_blocks, client.getListen(), request);
	if (server->m_body_size < request.getBodySize())
		return http_error(client, server->error_page, 413, 413);
	if (construct_response(response, *server, request, client,
				client_map, server_map))
	{
		return 0;
	}
	client.setResponse(response.get_raw());
	client.truncateRequest(client.getRequestSize());
	return 0;
}
