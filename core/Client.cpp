#include "Client.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

Client::Client(void) : _fd(-2), _flag(INCOMPLETE)
{}

Client::Client(Client const & src) : _fd(src._fd), _raw(src._raw),
									_listen(src._listen), _flag(src._flag)
{}

Client::~Client()
{}

Client &	Client::operator=(Client const & src)
{
	if (this == &src)
		return *this;
	_fd = src._fd;
	_raw = src._raw;
	_listen = src._listen;
	_flag = src._flag;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/

void	Client::eraseRaw(void) { _raw.clear(); }
void	Client::truncateRaw(const int len) { _raw.erase(0, len); }
void	Client::setResponse(const std::string& response, const int flag)
{ 
	eraseRaw();
	_raw = response;
	_flag = flag;
}


/*------------------------------GETTERS/SETTERS-------------------------------*/

void	Client::setFd(const int fd) { _fd = fd; }
void	Client::setRaw(const std::string& raw) { _raw = _raw + raw; }
void	Client::setRaw(const char *raw) { _raw = _raw + raw; }
void	Client::setListen(const std::pair<std::string, int>& listen)
		{ _listen = listen; }
void	Client::setFlag(const int flag) { _flag = flag; }


int									Client::getFd(void) const { return _fd; }
const std::string&					Client::getRaw(void) const { return _raw; }
const std::pair<std::string, int>&	Client::getListen(void) const
									{ return _listen; }
int									Client::getFlag(void) const { return _flag; }


/*------------------------------OVERLOAD OPERATORS----------------------------*/

std::ostream &operator<<(std::ostream &out, Client const &value)
{
	out << "[client with FD n: " << value.getFd() << " - listening IP: "
		<< value.getListen().first << " on port "
		<< value.getListen().second << std::endl << "Flag is set to "
		<< value.getFlag() << " and content is: "
		<< std::endl << value.getRaw() << "]" << std::endl;
	return (out);
}
