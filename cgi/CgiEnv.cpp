#include "CgiEnv.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

CgiEnv::CgiEnv(void)
{
	initMetaVar();
}

CgiEnv::CgiEnv(const Request& request,
                 		const Location_config& location_block,
                 		const Server_config& server_block,
                 		const Client& client)
{
	initMetaVar();
	setMetaVar(request, location_block, server_block, client);
}

CgiEnv::CgiEnv(CgiEnv const & src) : _metaVar(src._metaVar),
									_args(src._args)
{
	setEnv();
	setArgv();
}

CgiEnv::~CgiEnv()
{}

CgiEnv &	CgiEnv::operator=(CgiEnv const & src)
{
	if (this == &src)
		return *this;
	_metaVar = src._metaVar;
	_args = src._args;
	setEnv();
	setArgv();
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/

void	CgiEnv::initMetaVar()
{
	_env[METAVAR_NB] = NULL;
	_argv[MAX_ARG] = NULL;
	_argv[MAX_ARG - 1] = NULL;

	_metaVar.reserve(METAVAR_NB);
	_metaVar[SERVER_PROTOCOL] = "SERVER_PROTOCOL=HTTP/1.1";
	_metaVar[SERVER_SOFTWARE] = "SERVER_SOFTWARE=webserv";
	_metaVar[GATEWAY_INTERFACE] = "GATEWAY_INTERFACE=CGI/1.1";
	_metaVar[CONTENT_LENGHT] = "CONTENT_LENGHT=";
	_metaVar[CONTENT_TYPE] = "CONTENT_TYPE=";
	_metaVar[PATH_INFO] = "PATH_INFO=";
	_metaVar[QUERY_STRING] = "QUERY_STRING=";
	_metaVar[REQUEST_METHOD] = "REQUEST_METHOD=";
	_metaVar[REMOTE_ADDR] = "REMOTE_ADDR=";
	_metaVar[SCRIPT_NAME] = "SCRIPT_NAME=";
	_metaVar[SERVER_NAME] = "SERVER_NAME=";
	_metaVar[SERVER_PORT] = "SERVER_PORT=";
	_metaVar[PATH_TRANSLATED] = "PATH_TRANSLATED=";
}

void	CgiEnv::setEnv()
{
	for (int i = 0; i  < METAVAR_NB; i++)
		env[i] = const_cast<char*>(_metaVar[i].c_str());
}

//Sets the cstring array argv to use with execve()
//pathname must be formated as: /absolutepath/name. Ex: "/bin/ls"
//argv[0] must hold [name | path/name]. Ex: ["ls" | "bin/ls"]
//argv[n] holds arguments to the bin arguments. Ex: "-la"
//argv[end] must be NULL.
void	CgiEnv::setArgs(const Location_config& location_block)
{
	Location_config::c_cgi_map::const_iterator	find;

	find = location_block.cgi.find(request.getCgi());
	if (find == location_block.end())
		return ;
	_args.push_back(find->second);//get the cgi filesystem location from conf
	if (_metaVar[PATH_TRANSLATED].size() > 16)
		_args.push_back(_metaVar[PATH_TRANSLATED].substr(17));//pathinfo fs loc
}

void	CgiEnv::setArgv()
{
	if (_args.empty() == false)
		_argv[CGI_ROOT] = const_cast<char*>(_args[CGI_ROOT].c_str());
	if (_args.size() > 1)
		_argv[ABS_PATHINFO] = const_cast<char*>(_args[ABS_PATHINFO].c_str());
}

/*------------------------------GETTERS/SETTERS-------------------------------*/

// /foo/test.php/path/info?query=string 
//
// SCRIPTNAME->/foo/test.php
//trouver extension cgi dans target et ajouter a metavar
//
//PATH_INFO -> /var/www/website/users/path/info
//avec root /var/www/website/users/
//extraire path/info de la target http et la rajouter à root

void		CgiEnv::setMetaVar(const Request& request,
						const Location_config& location_block,
						const Server_config& server_block,
						const Client& client)
{
	size_t				find, find2;
	const std::string&	str;

	//REQUEST_METHOD
	_metaVar[REQUEST_METHOD].append(request.get_method());

	str = request.get_header("content-lenght");//CONTENT_LENGTH
	if (str.size() > 0)
		_metaVar[CONTENT_LENGTH].append(str);

	str = request.get_header("content-type");//CONTENT_TYPE
	if (str.size() > 0)
		_metaVar[CONTENT_TYPE].append(str);

	str = request.get_target();
	find = str.find_first_of('?');//QUERY_STRING
	if (find != std::string::npos && (find + 1) <= str.size())
		_metaVar[QUERY_STRING].append(str, find + 1, std::string::npos);

	find = str.find(request.getCgi());//SCRIPT_NAME
	if (find != std::string::npos)
	{
		find += (request.getCgi()).size();
		_metaVar[SCRIPT_NAME].append(str, 0, find - 1);
	}

	find2 = find;//PATH_INFO && PATH_TRANSLATED
	while (str.size() > find2 && str[find2] != '?')
		find2++;
	if ((find2 - find) > 1 && find2 < str.size())
	{
		_metaVar[PATH_TRANSLATED].append(location_block.root);
		_metaVar[PATH_TRANSLATED].append(str, find, find2);
		_metaVar[PATH_INFO].append(str, find, find2);
	}

	if (server_block.name_serv.empty() == false)//SERVER_NAME
		_metaVar[SERVER_NAME].append(server_block.name_serv[0]);
	else
		_metaVar[SERVER_NAME].append((client.getListen())->first);

	//SERVER_PORT
	_metaVar[SERVER_PORT].append((client.getListen())->second);

	//REMOTE_ADDR
	_metaVar[SERVER_PORT].append((client.getRemoteAddr()));

	setArgs(location_block);
	setEnv();
	setArgv();
}

char		**CgiEnv::getEnv(void) { return _env; }
char		**CgiEnv::getArgv(void) { return _argv; }

const std::vector<std::string>&
			CgiEnv::getMV(void) const { return _metaVar; }

std::ostream &operator<<(std::ostream &out, CgiEnv const &value)
{
	const std::vector<std::string>&	mv = value.getMV();

	for (int i = 0; i < METAVAR_NB; i++)
		out << mv[i] << std::endl;
	return (out);
}

