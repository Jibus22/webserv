#include "CgiEnv.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

CgiEnv::CgiEnv(void)
{
	initTables();
}

CgiEnv::CgiEnv(const Request& request,
                 		const Location_config& location_block,
                 		const Server_config& server_block,
                 		const Client& client,
						const std::string& ext_cgi)
{
	initTables();
	initMetaVar();
	setMetaVar(request, location_block, server_block, client, ext_cgi);
}

CgiEnv::CgiEnv(const CgiEnv& src) : _metaVar(src._metaVar),
									_args(src._args)
{
	initTables();
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

void	CgiEnv::initTables()
{
	memset(_env, 0, sizeof(char*) * METAVAR_NB + 1);
	memset(_argv, 0, sizeof(char*) * MAX_ARG + 1);
}

void	CgiEnv::initMetaVar()
{
	_metaVar.reserve(METAVAR_NB);
	_metaVar.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_metaVar.push_back("SERVER_SOFTWARE=webserv");
	_metaVar.push_back("GATEWAY_INTERFACE=CGI/1.1");
	_metaVar.push_back("CONTENT_LENGTH=");
	_metaVar.push_back("CONTENT_TYPE=");
	_metaVar.push_back("PATH_INFO=");
	_metaVar.push_back("QUERY_STRING=");
	_metaVar.push_back("REQUEST_METHOD=");
	_metaVar.push_back("REMOTE_ADDR=");
	_metaVar.push_back("SCRIPT_NAME=");
	_metaVar.push_back("SERVER_NAME=");
	_metaVar.push_back("SERVER_PORT=");
	_metaVar.push_back("PATH_TRANSLATED=");
	_metaVar.push_back("REDIRECT_STATUS=200");//Pour php-cgi...
}

//takes the address of each string of _metaVar vector to be formated as char**,
//to be compliant with execve() format parameters.
void	CgiEnv::setEnv()
{
	for (int i = 0; i  < METAVAR_NB; i++)
		_env[i] = const_cast<char*>(_metaVar[i].c_str());
}

//Sets _args vector to get correct strings for execve().
//-> CGI_ROOT, FILE_ROOT & optionally FILE_ARG
//Pathname and argv[0] can be the same. (absolute path to binary)
//argv[1] must contains the absolute path to the script to execute.
//argv[2] can contains path_translated wich would be an argument to the script.
//argv[end] must be NULL.
void	CgiEnv::setArgs(const Location_config& location_block,
						const std::string& ext_cgi)
{
	Location_config::c_cgi_map::const_iterator	match;
	std::string									script_path, script_name;

	match = location_block.cgi.find(ext_cgi);
	if (match == location_block.cgi.end())
		return ;
	_args.reserve(MAX_ARG);
	_args.push_back(match->second);//get the cgi filesystem location from conf
	script_path = location_block.root;
	script_name = (_metaVar[SCRIPT_NAME].substr(12));
	script_path.append(script_name, location_block.uri.size(), std::string::npos);
	_args.push_back(script_path);
	if (_metaVar[PATH_TRANSLATED].size() > 16)
		_args.push_back(_metaVar[PATH_TRANSLATED].substr(16));//pathinfo fs loc
}

//takes the address of each string of _args vector to be formated as char**, to be
//compliant with execve()'s argv format parameter.
void	CgiEnv::setArgv()
{
	if (_args.empty() == false)
		_argv[CGI_ROOT] = const_cast<char*>(_args[CGI_ROOT].c_str());
	if (_args.size() > 1)
		_argv[FILE_ROOT] = const_cast<char*>(_args[FILE_ROOT].c_str());
}

//set PATH_TRANSLATED. Map PATH_INFO with the correct location block then format
//it to the right filesystem path
int			CgiEnv::mapPath(const std::string& path_info,
						const Server_config& server_block)
{
	std::vector<Location_config*>::const_iterator	location, matching_location,
										end = server_block.location.end();
	size_t											pos = 0, len = 0, lenmax = 0;

	location = server_block.location.begin();
	matching_location = end;
	while (location != end)
	{
		pos = path_info.find((*location)->uri);
		if (pos == 0)
		{
			len = (*location)->uri.size();
			if (len > lenmax)
			{
				lenmax = len;
				matching_location = location;
			}
		}
		location++;
	}
	if (matching_location == end)
		return 1;
	_metaVar[PATH_TRANSLATED].append((*matching_location)->root);
	_metaVar[PATH_TRANSLATED].append(path_info, lenmax);
	return 0;
}

//Set SCRIPT_NAME, and PATH_INFO + PATH_TRANSLATED if any.
int			CgiEnv::setPath(const std::string& target,
						const std::string& ext_cgi,
						const Server_config& server_block)
{
	size_t		match, match2;
	std::string	path_info;

	match = target.find(ext_cgi);//SCRIPT_NAME
	if (match == std::string::npos)
		return 1;
	match += ext_cgi.size();
	_metaVar[SCRIPT_NAME].append(target, 0, match);
	match2 = match;//PATH_INFO && PATH_TRANSLATED
	while (target.size() > match2 && target[match2] != '?')
		match2++;
	if (match2 == match)
		return 0;
	path_info.assign(target, match, match2 - match);
	_metaVar[PATH_INFO].append(path_info);//perhaps url-decode it
	mapPath(path_info, server_block);
	return 0;
}


/*------------------------------GETTERS/SETTERS-------------------------------*/

//Sets all mandatory CGI/1.1 meta-variables
void		CgiEnv::setMetaVar(const Request& request,
						const Location_config& location_block,
						const Server_config& server_block,
						const Client& client,
						const std::string& ext_cgi)
{
	std::map<std::string, std::string>::const_iterator	hdr;
	const std::string&	target = request.get_target();
	size_t				match;
	std::stringstream	ss;
	bool				found;

	initMetaVar();
	_metaVar[REQUEST_METHOD].append(request.get_method());
	ss << (client.getListen()).second;
	_metaVar[SERVER_PORT].append(ss.str());
	_metaVar[REMOTE_ADDR].append(client.getRemoteAddr());
	if (server_block.name_serv.empty() == false)
		_metaVar[SERVER_NAME].append(server_block.name_serv[0]);
	else
		_metaVar[SERVER_NAME].append((client.getListen()).first);
	hdr = request.get_header("content-length", found);
	if (found == true)
		_metaVar[CONTENT_LENGTH].append(hdr->second);
	hdr = request.get_header("content-type", found);
	if (found == true)
		_metaVar[CONTENT_TYPE].append(hdr->second);

	match = target.find_first_of('?');
	if (match != std::string::npos && (match + 1) <= target.size())
		_metaVar[QUERY_STRING].append(target, match + 1, std::string::npos);
	setPath(target, ext_cgi, server_block);
	setEnv();

	setArgs(location_block, ext_cgi);
	setArgv();
}

char		**CgiEnv::getEnv(void) { return _env; }
char		**CgiEnv::getArgv(void) { return _argv; }

const std::vector<std::string>&
			CgiEnv::getMV(void) const { return _metaVar; }

const std::vector<std::string>&
			CgiEnv::getArgs(void) const { return _args; }

std::ostream &operator<<(std::ostream &out, const CgiEnv& value)
{
	const std::vector<std::string>&	mv = value.getMV();
	const std::vector<std::string>&	mv2 = value.getArgs();

	for (int i = 0; i < METAVAR_NB; i++)
		out << mv[i] << std::endl;
	for (std::vector<std::string>::const_iterator i = mv2.begin();
			i != mv2.end(); i++)
		out << *i << std::endl;
	return (out);
}
