#include "webserv.hpp"

void	process_cgi(Response & response,
					const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client,
					const std::string & cgi_ext)
{
	CgiEnv	env(request, location_block, server_block, client, cgi_ext);
	(void)response;

	__D_DISPLAY(env);
	//execve();
}
//int execve(const char *pathname, char *const argv[], char *const envp[]);

//pathname must be formated as: /absolutepath/name. Ex: "/bin/ls"
//argv[0] must hold [name | path/name]. Ex: ["ls" | "bin/ls"]
//argv[n] holds arguments to the bin arguments. Ex: "-la"
//argv[end] must be NULL.
