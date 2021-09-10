#ifndef CGIENV_HPP
#define CGIENV_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "webserv.hpp"

# define METAVAR_NB 13

# define SERVER_PROTOCOL 0
# define SERVER_SOFTWARE 1
# define GATEWAY_INTERFACE 2
# define CONTENT_LENGTH 3
# define CONTENT_TYPE 4
# define PATH_INFO 5
# define QUERY_STRING 6
# define REQUEST_METHOD 7
# define REMOTE_ADDR 8
# define SCRIPT_NAME 9
# define SERVER_NAME 10
# define SERVER_PORT 11
# define PATH_TRANSLATED 12


# define MAX_ARG 3
# define CGI_ROOT 0
# define FILE_ROOT 1
# define FILE_ARG 2
//CgiEnv is a class made to build a CGI/1.1 environment & resolve the location
//of the cgi in the filesystem, to be able to call execve() properly.
//It sets meta-variables accordingly to the needs of CGI/1.1 scripts.
class CgiEnv
{
	std::vector<std::string>	_metaVar;
	char						*_env[METAVAR_NB + 1];
	std::vector<std::string>	_args;
	char						*_argv[MAX_ARG + 1];

public:
	CgiEnv();
	CgiEnv(const CgiEnv& src);
	CgiEnv(const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client,
					const std::string& ext_cgi);
	~CgiEnv();
	CgiEnv&	operator=(const CgiEnv& src);

	void		setMetaVar(const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client,
					const std::string& ext_cgi);

	char							**getEnv();
	char							**getArgv();
	const std::vector<std::string>&	getMV() const;
	const std::vector<std::string>&	getArgs() const;

private:
	void		initMetaVar(void);
	void		initTables();
	int			setPath(const std::string& target,
						const std::string& ext_cgi,
						const Server_config& server_block);
	int			mapPath(const std::string& path_info,
						const Server_config& server_block);
	void		setEnv(void);
	void		setArgs(const Location_config& location_block,
						const std::string& ext_cgi);
	void		setArgv();
};

std::ostream &operator<<(std::ostream &out, CgiEnv const &value);

#endif
