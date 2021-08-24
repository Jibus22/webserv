#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "webserv.hpp"

class Client
{
private:
	int							_fd;
	std::string					_raw;
	std::pair<std::string, int>	_listen;
	int							_flag;
public:
/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

						Client();
						Client(Client const & src);
						~Client();
	Client &			operator=(Client const & src);

/*----------------------------------METHODS-----------------------------------*/

	void				eraseRaw(void);


/*------------------------------GETTERS/SETTERS-------------------------------*/

	void				setFd(const int fd);
	void				setRaw(const std::string& raw);
	void				setRaw(const char *raw);
	void				setListen(const std::pair<std::string, int>& listen);
	void				setFlag(const int flag);

	int									getFd(void) const;
	const std::string &					getRaw(void) const;
	const std::pair<std::string, int>&	getListen(void) const;
	int									getFlag(void) const;
};

std::ostream &operator<<(std::ostream &out, Client const &value);

#endif
