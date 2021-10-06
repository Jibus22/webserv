#include "Request.hpp"
#include "webserv.hpp"

/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

//Fulfill all private variables (start line, headers, body)
Request::Request(std::string const& request) : _request(request)
{
	std::string	*start_line[3] = {&_method, &_target, &_version};
	size_t	start = 0, endline = 0, separator = 0;

	_blankline = request.find("\r\n\r\n");
	for (size_t i = 0; i < 3; i++)
	{
		if (i < 2)
			separator = request.find(' ', start);
		else
			separator = request.find('\n', start);
		*(start_line[i]) = request.substr(start, separator - start);
		start = separator + 1;
	}
	while (endline < _blankline)
	{
		separator = request.find(':', start);
		endline = request.find('\n', separator);
		_headers[str_to_lower(request.substr(start, separator - start))]
			= request.substr(separator + 1, endline - (separator + 1));
		start = endline + 1;
	}
	_path = _target;
}

Request::Request(Request const & src) : _request(src._request)
{
	*this = src;
	return;
}

Request &	Request::operator=(Request const & rhs){
	this->_method = rhs._method;
	this->_target = rhs._target;
	this->_headers = rhs._headers;
	this->_path = rhs._path;
	return *this;
}


Request::~Request(void){
	return;
}


/*------------------------------GETTERS/SETTERS-------------------------------*/

std::map<std::string, std::string>::const_iterator
		Request::getHeader(const std::string& key_header, bool& found) const
{
	std::map<std::string, std::string>::const_iterator
			header(_headers.find(key_header));

	if (header == _headers.end())
		found = false;
	else
		found = true;
	return header;
}

bool
Request::getHeader(const std::string& key, std::string& value) const
{
	std::map<std::string, std::string>::const_iterator
			header(_headers.find(key));

	if (header == _headers.end())
		return false;
	value = header->second;
	return true;
}

std::map<std::string, std::string> const&
					Request::get_headers() const {return this->_headers;}

std::string const&	Request::get_method() const {return this->_method;}
std::string const&	Request::get_target() const {return this->_target;}
std::string const&	Request::get_version() const {return this->_version;}
const std::string&	Request::getRequest() const {return _request;}
size_t				Request::getBodyPos() const {return _blankline + 4;}
size_t				Request::getBodySize() const
					{return _request.size() - (_blankline + 4);}
const char*			Request::getBodyAddr() const
					{return &(_request[_blankline + 4]);}
std::string &		Request::getPath()  {return _path;}
std::string const&	Request::getPath() const {return _path;}


void	Request::setTarget(const std::string& newtarget)
		{_target = newtarget;}
void	Request::setPath(const std::string& uri, const std::string& root)
{
	(void)uri;
	(void)root;
}

/*------------------------------OVERLOAD OPERATORS----------------------------*/

const std::string&	Request::operator[](std::string const & key_header)
{return this->_headers[key_header];}

std::ostream &operator<<(std::ostream &out, const Request& value)
{
	const std::map<std::string, std::string>&	hdr = value.get_headers();

	out << WHITE_C << "[REQUEST]\n" <<
		"status line: " << RESET_C << value.get_method() << " "
		<< value.get_target() << " "
		<< value.get_version() << "\n"
		<< WHITE_C << "header map:\n" << RESET_C;
	for (std::map<std::string, std::string>::const_iterator it = hdr.begin();
			it != hdr.end(); it++)
	{
		out << it->first << ":" << it->second << "\n";
	}
	out << WHITE_C << "[hdr end]\nbody len: " << RESET_C << value.getBodySize();
	return out;
}

/*------------------------------EXCEPTIONS------------------------------------*/

/*
const char* Request::NotTerminatedException::what() const throw()
{
	return "Request not Terminated";
}

const char* Request::InvalidRequest::what() const throw()
{
	return "Invalid Request";
}

void Request::add_header(std::string const & header)
{
	size_t pos_separateur = header.find(":");

	if (pos_separateur == std::string::npos)
		throw InvalidRequest();
	else
	{
		std::string header_min = str_to_lower(header.substr(0, pos_separateur));
		if(header[pos_separateur + 1] == ' ')
			this->_headers[header_min] =
			header.substr(pos_separateur + 2);
		else
			this->_headers[header_min] =
			header.substr(pos_separateur + 1);
	}
}


void Request::parse_first_line(std::string const & first_line)
{
	std::vector<std::string> words;
	split(words, first_line, ' ');

	if(words.size() != 3)
		throw InvalidRequest();
	this->_method = words[0];
	this->_target = words[1];
	this->_version = words[2];
}

void	Request::checkTerminatedBody()
{
	std::stringstream str(this->_headers["Content-Length"]);
	unsigned long length;

	str >> length;
	if (!str)
		throw InvalidRequest();
	if (this->_body.size() != length)
		throw NotTerminatedException();
}*/

/*
// TODO:  Exception a gerer si requete mal formate
Request::Request(std::string const & raw_r)
{
	//__D_DISPLAY(raw_r);
	std::string raw_request(raw_r);
	//stocke le body et le supprime de la string
	size_t pos_body = raw_request.find("\r\n\r\n");
	if (pos_body == std::string::npos)
	{
		__D_DISPLAY("Pas de \\r\\n\\r\\n -> Requete non terminé");
		throw Request::NotTerminatedException();
	}
	else
	{
		this->_body = raw_request.substr(pos_body + 4);
		raw_request.erase(pos_body);
	}

	//Separe chaque ligne de la requete
	std::vector<std::string> lines;
	split(lines, raw_request, '\n');
	std::vector<std::string>::iterator it = lines.begin();
	std::vector<std::string>::iterator ite = lines.end();

	if(it != ite)
		parse_first_line(*(it++));
	else
		throw InvalidRequest();
	//TODO: else ligne vide -> exception

	while(it != ite)
	{
		this->add_header(*it);
		it++;
	}

	//check si body est fini sinon NotTerminatedException
	//if (!this->_body.empty())
		//this->checkTerminatedBody();
	//Matéo, cette fonction  ^ me renvoie une exception alors que ça devrait pas
	//Du coup je l'ai commentée pour le moment. Si j'ai bien lu le code
	//il m e semble que cette vérification est déjà faie desormais dans mon
	//pré-parsing
}*/

/*
void split(std::vector<std::string> & output, const std::string& s,
													char const separator)
{
	std::string::size_type prev_pos = 0, pos = 0;

	while((pos = s.find(separator, pos)) != std::string::npos)
	{
        output.push_back(s.substr(prev_pos, pos - prev_pos));
        prev_pos = ++pos;
	}
    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
}*/

/*
** CANONICAL FUNCS
*/
/*
Request::Request(void){
	return;
}*/
