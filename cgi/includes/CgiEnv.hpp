#ifndef CGIENV_HPP
#define CGIENV_HPP

#include "Request.hpp"
#include "Client.hpp"
#include "CgiMetaVar.hpp"
#include <string>
#include <vector>

# define METAVAR_NB 12

# define SERVER_PROTOCOL 0
# define SERVER_SOFTWARE 1
# define GATEWAY_INTERFACE 2
# define CONTENT_LENGHT 3
# define CONTENT_TYPE 4
# define PATH_INFO 5
# define QUERY_STRING 6
# define REQUEST_METHOD 7
# define REMOTE_ADDR 8
# define SCRIPT_NAME 9
# define SERVER_NAME 10
# define SERVER_PORT 11

class CgiEnv
{
	std::vector<std::string>	_metaVar;
	const char					*_env[METAVAR_NB];

public:
	CgiEnv();
	CgiEnv(const CgiEnv& src);
	CgiEnv(const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client);
	~CgiEnv();
	CgiEnv&	operator=(const CgiEnv& src);

	void		setMetaVar(const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client);

	const char						*getEnv() const;
	const std::vector<std::string>&	getMV() const;

private:
	void		initMetaVar(void);
	void		setEnv(void);
};

std::ostream &operator<<(std::ostream &out, CgiEnv const &value);

#endif
