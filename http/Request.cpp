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
std::string const&	Request::getTarget() const {return this->_target;}
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
	_path = _target;
	if (uri[uri.size() - 1] == '/'
			|| uri.size() == 1)
		_path.erase(0 , uri.size());
	else
		_path.erase(0 , uri.size() + 1);
	if (root[root.size() - 1] == '/')
		_path.insert(0, root);
	else
		_path.insert(0, root + "/");
}

/*------------------------------OVERLOAD OPERATORS----------------------------*/

const std::string&	Request::operator[](std::string const & key_header)
{return this->_headers[key_header];}

std::ostream &operator<<(std::ostream &out, const Request& value)
{
	const std::map<std::string, std::string>&	hdr = value.get_headers();

	out << WHITE_C << "[REQUEST]\n" <<
		"status line: " << RESET_C << value.get_method() << " "
		<< value.getTarget() << " "
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
