#include "CgiEnv.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

CgiEnv::CgiEnv(void) : _metaVar(METAVAR_NB)
{
	initMetaVar();
}

CgiEnv::CgiEnv(CgiEnv const & src) : _metaVar(src._metaVar)
{
	setEnv();
}

CgiEnv::~CgiEnv()
{}

CgiEnv &	CgiEnv::operator=(CgiEnv const & src)
{
	if (this == &src)
		return *this;
	_metaVar = src._metaVar;
	setEnv();
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/

void	CgiEnv::initMetaVar()
{
	_metaVar[SERVER_PROTOCOL] = "SERVER_PROTOCOL = HTTP/1.1";
	_metaVar[SERVER_SOFTWARE] = "SERVER_SOFTWARE = webserv";
	_metaVar[GATEWAY_INTERFACE] = "GATEWAY_INTERFACE = CGI/1.1";
	_metaVar[CONTENT_LENGHT] = "CONTENT_LENGHT = ";
	_metaVar[CONTENT_TYPE] = "CONTENT_TYPE = ";
	_metaVar[PATH_INFO] = "PATH_INFO = ";
	_metaVar[QUERY_STRING] = "QUERY_STRING = ";
	_metaVar[REQUEST_METHOD] = "REQUEST_METHOD = ";
	_metaVar[REMOTE_ADDR] = "REMOTE_ADDR = ";
	_metaVar[SCRIPT_NAME] = "SCRIPT_NAME = ";
	_metaVar[SERVER_NAME] = "SERVER_NAME = ";
	_metaVar[SERVER_PORT] = "SERVER_PORT = ";
}

void	CgiEnv::setEnv()
{
	for (int i = 0; i  < METAVAR_NB; i++)
		env[i] = _metaVar[i].c_str();
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

void	CgiEnv::setMetaVar(const Request& request,
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

	find2 = find;//PATH_INFO
	while (str.size() > find2 && str[find2] != '?')
		find2++;
	if ((find2 - find) > 1 && find2 < str.size())
	{
		_metaVar[PATH_INFO].append(location_block.root);
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

	setEnv();
}

const char	*CgiEnv::getEnv(void) const { return _env; }

const std::vector<std::string>&
			CgiEnv::getMV(void) const { return _metaVar; }

std::ostream &operator<<(std::ostream &out, CgiEnv const &value)
{
	const std::vector<std::string>&	mv = value.getMV();

	out
	<< mv[SERVER_PROTOCOL] << std::endl
	<< mv[SERVER_SOFTWARE] << std::endl
	<< mv[GATEWAY_INTERFACE] << std::endl
	<< mv[CONTENT_LENGHT] << std::endl
	<< mv[CONTENT_TYPE] << std::endl
	<< mv[PATH_INFO] << std::endl
	<< mv[QUERY_STRING] << std::endl
	<< mv[REQUEST_METHOD] << std::endl
	<< mv[REMOTE_ADDR] << std::endl
	<< mv[SCRIPT_NAME] << std::endl
	<< mv[SERVER_NAME] << std::endl
	<< mv[SERVER_PORT] << std::endl;
	return (out);
}

