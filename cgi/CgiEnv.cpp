#include "CgiEnv.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

CgiEnv::CgiEnv(void)
{}

CgiEnv::CgiEnv(CgiEnv const & src) { *this = src; }

CgiEnv::~CgiEnv()
{}

CgiEnv &	CgiEnv::operator=(CgiEnv const & src)
{
	if (this == &src)
		return *this;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/



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
					const Location_config& location_block
					const Server_config& server_block)
					const Client& client)
{
	size_t				find, find2;
	const std::string&	str;

	_metaVar._request_method.append(request.get_method());//REQUEST_METHOD

	str = request.get_header("content-lenght");//CONTENT_LENGTH
	if (str.size() > 0)
		_metaVar._content_length.append(str);

	str = request.get_header("content-type");//CONTENT_TYPE
	if (str.size() > 0)
		_metaVar._content_type.append(str);

	str = request.get_target();
	find = str.find_first_of('?');//QUERY_STRING
	if (find != std::string::npos && (find + 1) <= str.size())
		_metaVar._query_string.append(str, find + 1, std::string::npos);

	find = str.find(request.getCgi());//SCRIPT_NAME
	if (find != std::string::npos)
	{
		find += (request.getCgi()).size();
		_metaVar._script_name.append(str, 0, find - 1);
	}

	find2 = find;//PATH_INFO
	while (str.size() > find2 && str[find2] != '?')
		find2++;
	if ((find2 - find) > 1 && find2 < str.size())
	{
		_metaVar._path_info.append(location_block.root);
		_metaVar._path_info.append(str, find, find2);
	}

	if (server_block.name_serv.empty() == false)//SERVER_NAME
		_metaVar._server_name.append(server_block.name_serv[0]);
	else
		_metaVar._server_name.append((client.getListen())->first);

	_metaVar._server_port.append((client.getListen())->second);//SERVER_PORT

	_metaVar._server_port.append((client.getRemoteAddr()));//REMOTE_ADDR
}

std::ostream &operator<<(std::ostream &out, CgiEnv const &value)
{
	//out << "I am " << value.getName() << std::endl;
	return (out);
}

