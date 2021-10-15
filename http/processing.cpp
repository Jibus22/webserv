#include "processing.hpp"

//return true if server_name of server config matchs
//with request host header value
bool	match_server_name(const Server_config& server, const Request& request)
{
	std::string	value;
	size_t		pos;

	if (!request.getHeader("host", value))
		return false;
	if (value.size() < 2)
		return false;
	pos = value.find(':');
	if (pos != std::string::npos)
		value.erase(pos);
	pos = value.find_first_not_of(' ');
	if (pos > 0)
		value.erase(0, pos);
	for (std::vector<std::string>::const_iterator
			it = server.name_serv.begin(); it != server.name_serv.end(); it++)
		if (value == *it)
			return true;
	return false;
}

//First fetch a vector of server_config which has there ip:port pair matching
//with the one from the client.
//Then try to make each of them matching with a more specific server_name
Server_config	*match_server(const std::vector<Server_config*>& server_blocks,
					const std::pair<std::string, int>& listen,
					const Request& request)
{
	std::vector<Server_config*>	listen_match;

	for (std::vector<Server_config*>::const_iterator it = server_blocks.begin();
		it != server_blocks.end(); it++)
		if((*it)->listen == listen)
			listen_match.push_back(*it);
	if (listen_match.size() == 1)
		return listen_match.front();
	else if (listen_match.size() > 1)
	{
		for (std::vector<Server_config*>::const_iterator
				it = listen_match.begin(); it != listen_match.end(); it++)
			if (match_server_name(**it, request))
				return *it;
		return listen_match.front();
	}
	else
		return listen_match.front();
}

// Trouve la location associé a l'URI de la requete
//[1.] : cas ou location uri fini par /
//on compare jusque avant le / de la location URI
//ca match si compare == 0 et si soit la target est fini apres la comparaison
// soit si il y a un / apres
//Par exemple /dir/
//doit matcher avec /dir /dir/ /dir/test.html
// mais pas /directory
//[2.] : cas ou location fini pas par /
//pareil mais longueur de comparaison change
Location_config	*location_match(const Config_struct::c_location_vector&
									locations, const std::string& target)
{
	Config_struct::c_location_vector::const_iterator
					it = locations.begin(),
					end = locations.end();
	Location_config	*location;

	if (locations.empty())
		return NULL;
	while (it != end)
	{
		location = *it;
		if (location->uri == "/")
			break ;
		else if (location->uri[location->uri.size() - 1] == '/')//[1.]
		{
			if((target.compare(0, location->uri.size() - 1,
			location->uri, 0, location->uri.size() - 1) == 0)
			&& (target.size() == location->uri.size() - 1 ||
				target[location->uri.size() - 1] == '/'))
				break ;
		}
		else//[2.]
		{
			if((target.compare(0, location->uri.size(), location->uri) == 0) &&
					(target.size() == location->uri.size()
					 || target[location->uri.size()] == '/'))
				break ;
		}
		it++;
	}
	if (it == end)
		return NULL;
	__D_DISPLAY("location matched : " << location->uri);
	return location;
}

//Return true if HTTP method from request is found as allowed in the
//location config block, else return false.
bool	is_method_allowed(Location_config * location, std::string methode)
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
	const std::string&	target = request.getTarget();
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

//Set target to index file declared in location block from the config file if
//it's found & if it exists into the local filesystem.
void	handle_index(std::string& target, const Location_config& location)
{
	__D_DISPLAY("DIRECTORY");
	for (std::vector<std::string>::const_iterator it = location.index.begin();
			it != location.index.end(); it++)
	{
		if (is_file_exist(target + *it))
		{
			target.append(*it);
			return;
		}
	}
}


//Wether the http target requests for a directory or a file, wether config
//directives as 'autoindex on;' or 'index file;' are found or not, process
//or get the file then set the according http response.
int		http_get(Request& request, const Server_config& server,
				const Location_config& location, Client& client, int ret)
{
	size_t	filesize;

	if (is_dir(request.getPath()))
		handle_index(request.getPath(), location);
	if (is_dir(request.getPath()) && location.auto_index == true)
		return http_response(client, auto_index(request.getPath(),
					request.getTarget()), 200, 1);
	else if (!is_dir(request.getPath()) && is_openable(request.getPath()))
	{
		filesize = get_file_size(request.getPath().c_str());
		if (ret > 1)
			return http_response(client, request.getTarget(), 302, 1,
					request.getPath(), filesize);
		else
			return http_response(client, "", 200, 1,
					request.getPath(), filesize);
	}
	else
		return http_error(client, server.error_page, 404, 404);
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
						request.getTarget());
	else
		return http_error(client, server.error_page, 415, 1);
}

int		http_delete(Client& client, const Request& request,
				const Server_config& server)
{
	if (is_dir(request.getPath()))
		return http_error(client, server.error_page, 400, 400);
	else if (!is_openable(request.getPath()))
		return http_error(client, server.error_page, 404, 404);
	if (unlink(request.getPath().c_str()) == -1)
		return http_error(client, server.error_page, 500, 500);
	else
		return http_response(client, "", 200, 200);
}

//Set 'Allow' header value accordingly then returns error response to 405
//Method Not Allowed
int		unallowed_method(Client& client, const Location_config& location,
				const Server_config& server)
{
	std::string	value("");

	for (std::vector<std::string>::const_iterator it = location.methode.begin();
			it != location.methode.end(); it++)
		value.append(*it + ", ");
	if (!value.empty())
		value.erase(value.size() - 2);
	return http_error(client, server.error_page, value, 405, 405);
}

int		construct_response(Server_config& server,
				Request& request, Client& client,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{

	int				ret = CGI_REDIRECT, i = 0;
	Location_config	*location;

	while (ret == CGI_REDIRECT)
	{
		location = location_match(server.location, request.getTarget());
		if (!location)
			return http_error(client, server.error_page, 404, 1);
		if (location->return_p.first != 0)
			return http_response(client, location->return_p.second,
					location->return_p.first, location->return_p.first);
		ret = check_cgi(request, server, *location, client,
				client_map, server_map);
		i++;
	}
	if (ret == CGI_SUCCESS || ret == CGI_ERR)
		return 1;
	__D_DISPLAY("target : " << request.getTarget());
	request.setPath(location->uri, location->root);
	__D_DISPLAY("path : " << request.getPath());
	if (request.get_method() == "GET" && is_method_allowed(location, "GET"))
		return http_get(request, server, *location, client, i);
	else if (request.get_method() == "POST" &&
			is_method_allowed(location, "POST"))
		return http_post(client, request, *location, server);
	else if(request.get_method() == "DELETE" &&
			is_method_allowed(location, "DELETE"))
		return http_delete(client, request, server);
	else
		return unallowed_method(client, *location, server);
}

int		process_request(Client& client,
				const std::vector<Server_config*>& server_blocks,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	Server_config	*server;
	Request 		request(client.getStrRequest());

	__D_DISPLAY(request);
	server = match_server(server_blocks, client.getListen(), request);
	if (server->m_body_size < request.getBodySize())
		return http_error(client, server->error_page, 413, 413);
	construct_response(*server, request, client, client_map, server_map);
	return 0;
}
