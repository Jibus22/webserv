#include "Client.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

Client::Client(void) : _fd(-2), _flag(INCOMPLETE)
{}

Client::Client(Client const & src) : _fd(src._fd), _request(src._request),
									_listen(src._listen), _flag(src._flag)
{}

Client::~Client()
{}

Client &	Client::operator=(Client const & src)
{
	if (this == &src)
		return *this;
	_fd = src._fd;
	_request = src._request;
	_listen = src._listen;
	_flag = src._flag;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/

void	Client::truncateRequest(const int len) { _request.erase(0, len); }
void	Client::truncateResponse(const int len) { _response.erase(0, len); }
void	Client::clearResponse(void) { _response.clear(); _flag = INCOMPLETE; }


/*------------------------------GETTERS/SETTERS-------------------------------*/

void	Client::setFd(const int fd) { _fd = fd; }
void	Client::setListen(const std::pair<std::string, int>& listen)
		{ _listen = listen; }
void	Client::setFlag(const int flag) { _flag = flag; }

void	Client::setRequest(const char *request, const int len)
		{ _request.append(request, request + len); }

void	Client::setResponse(const char *begin, const char *end, const int flag)
{ 
	_response.assign(begin, end);
	_flag = flag;
}


int					Client::getFd(void) const { return _fd; }
const std::pair<std::string, int>&
					Client::getListen(void) const { return _listen; }
int					Client::getFlag(void) const { return _flag; }

size_t				Client::getRequestSize(void) const
					{ return _request.size(); }
const std::string&	Client::getStrRequest(void) const { return _request; }
const char*			Client::getRawRequest(void) const { return _request.data(); }

size_t				Client::getResponseSize(void) const
					{ return _response.size(); }
const std::string&	Client::getStrResponse(void) const { return _response; }
const char*			Client::getRawResponse(void) const
					{ return _response.data(); }



/*------------------------------OVERLOAD OPERATORS----------------------------*/

std::ostream &operator<<(std::ostream &out, Client const &value)
{
	out << "[Client " << value.getFd() << " ("
		<< value.getListen().first << ":"
		<< value.getListen().second << ") Stat: "
		<< value.getFlag() << "]" << std::endl << "[Request:"
		<< std::endl << value.getStrRequest() << "(END)]" << std::endl;
	return (out);
}
