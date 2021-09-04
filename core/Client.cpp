#include "Client.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

Client::Client(void) : _fd(-2), _ready(false)
{}

Client::Client(Client const & src) : _fd(src._fd), _listen(src._listen),
									_request(src._request), _ready(src._ready),
									_remote_address(src._remote_address)
{}

Client::~Client()
{}

Client &	Client::operator=(Client const & src)
{
	if (this == &src)
		return *this;
	_fd = src._fd;
	_listen = src._listen;
	_request = src._request;
	_ready = src._ready;
	_remote_address = src._remote_address;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/

void	Client::truncateRequest(const int len) { _request.erase(0, len); }
void	Client::truncateRequest(const char *end)
		{
			const char	*begin = _request.data();
			_request.erase(0, (end - begin));
		}
void	Client::truncateResponse(const int len)
		{ (_qResponse.front())->erase(0, len); }
void	Client::clearResponse(void)
{
	delete _qResponse.front();
	_qResponse.pop();
	_ready = false;
}


/*------------------------------GETTERS/SETTERS-------------------------------*/

void	Client::setFd(const int fd) { _fd = fd; }
void	Client::setListen(const std::pair<std::string, int>& listen)
		{ _listen = listen; }
void	Client::setReady(void) { _ready = true; }

void	Client::setRemoteAddr(const char *remote_addr)
		{ _remote_address.assign(remote_addr); }

void	Client::setRequest(const char *request, const int len)
		{ _request.append(request, request + len); }

void	Client::setResponse(std::string *response)
		{ _qResponse.push(response); }


size_t				Client::getResponseNb(void) const
					{ return _qResponse.size(); }
int					Client::getFd(void) const { return _fd; }
const std::pair<std::string, int>&
					Client::getListen(void) const { return _listen; }
bool				Client::isReady(void) const { return _ready; }
const std::string&	Client::getRemoteAddress(void) const
					{ return _remote_address; }

int					Client::getRequestSize(void) const
					{ return _request.size(); }
const std::string&	Client::getStrRequest(void) const { return _request; }
const char*			Client::getRawRequest(void) const { return _request.data(); }

size_t				Client::getResponseSize(void) const
{
	if (_qResponse.empty())
		return 0;
	return (_qResponse.front())->size();
}
const std::string&	Client::getStrResponse(void) const
					{ return *(_qResponse.front()); }
const char*			Client::getRawResponse(void) const
					{ return (_qResponse.front())->data(); }



/*------------------------------OVERLOAD OPERATORS----------------------------*/

std::ostream &operator<<(std::ostream &out, Client const &value)
{
	out << "[Client " << value.getFd() << " ("
		<< value.getListen().first << ":"
		<< value.getListen().second << ") Stat: "
		<< value.isReady() << "]" << std::endl << "[Request:"
		<< std::endl << value.getStrRequest() << "(END)]" << std::endl;
	return (out);
}
