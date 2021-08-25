#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <utility>
#include <iostream>
#include "macros.hpp"

class Client
{
private:
	int							_fd;
	std::string					_request;
	std::string					_response;
	std::pair<std::string, int>	_listen;
	int							_flag;
public:
/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

				Client();
				Client(Client const & src);
				~Client();
	Client &	operator=(Client const & src);

/*----------------------------------METHODS-----------------------------------*/

	void		truncateRequest(const int len);
	void		truncateResponse(const int len);
	void		clearResponse(void);


/*------------------------------GETTERS/SETTERS-------------------------------*/

	void		setFd(const int fd);
	void		setListen(const std::pair<std::string, int>& listen);
	void		setFlag(const int flag);

	void		setRequest(const char *request, const int len);

	void		setResponse(const char *begin, const char *end, const int flag);


	int									getFd(void) const;
	const std::pair<std::string, int>&	getListen(void) const;
	int									getFlag(void) const;

	size_t								getRequestSize() const;
	const std::string &					getStrRequest(void) const;
	const char*							getRawRequest(void) const;

	size_t								getResponseSize() const;
	const std::string &					getStrResponse(void) const;
	const char*							getRawResponse(void) const;
};

std::ostream &operator<<(std::ostream &out, Client const &value);

#endif
