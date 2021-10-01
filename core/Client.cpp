#include "Client.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

Client::Client(void) : _fd(-2), _blankline(-1), _contentlen(-1),
						_offset(0), _ready(false)
{}

Client::Client(Client const & src) : _fd(src._fd), _listen(src._listen),
							_remote_address(src._remote_address),
							_request(src._request), _blankline(src._blankline),
							_contentlen(src._contentlen), _offset(src._offset),
							_ready(src._ready)
{}

Client::~Client()
{
	while (_qResponse.empty() == false)
	{
		delete _qResponse.front();
		_qResponse.pop();
	}
}

Client &	Client::operator=(Client const & src)
{
	if (this == &src)
		return *this;
	_fd = src._fd;
	_blankline = src._blankline;
	_contentlen = src._contentlen;
	_listen = src._listen;
	_request = src._request;
	_ready = src._ready;
	_offset = src._offset;
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
void	Client::setOffset(const ssize_t len) { _offset += len; }
void	Client::clearRequest(void)
{
	std::string	str("");

	_blankline = -1;
	_contentlen = -1;
	_request.swap(str);
}
void	Client::clearResponse(void)
{
	delete _qResponse.front();
	_qResponse.pop();
	_offset = 0;
	_ready = false;
}


/*------------------------------GETTERS/SETTERS-------------------------------*/

void	Client::setFd(const int fd) { _fd = fd; }
void	Client::setListen(const std::pair<std::string, int>& listen)
		{ _listen = listen; }
void	Client::setBlankLine(const int pos) { _blankline = pos; }
void	Client::setContentLen(const int len)
{
	_request.reserve(len);
	_contentlen = len;
}
void	Client::setReady(void) { _ready = true; }

void	Client::setRemoteAddr(const std::string& remote_addr)
		{ _remote_address.assign(remote_addr); }

void	Client::setRequest(const char *request, const int len)
		{ _request.append(request, request + len); }

void	Client::setResponse(std::string *response)
		{ _qResponse.push(response); }


bool	Client::isBlankLine(void) const { return _blankline >= 0; }
bool	Client::isContentLen(void) const { return _contentlen >= 0; }


int					Client::getBlankLine(void) const { return _blankline; }
int					Client::getContentLen(void) const { return _contentlen; }
int					Client::getBodyLen(void) const
					{ return _request.size() - (_blankline + 4); }


size_t				Client::getResponseNb(void) const
					{ return _qResponse.size(); }
int					Client::getFd(void) const { return _fd; }
const std::pair<std::string, int>&
					Client::getListen(void) const { return _listen; }
bool				Client::isReady(void) const { return _ready; }
const std::string&	Client::getRemoteAddr(void) const
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
size_t				Client::getLenToSend(void) const
{
	if (_qResponse.empty())
		return 0;
	return (_qResponse.front())->size() - _offset;
}
const std::string&	Client::getStrResponse(void) const
					{ return *(_qResponse.front()); }
const char*			Client::getRawResponse(void) const
					{ return (_qResponse.front())->data() + _offset; }



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
