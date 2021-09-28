#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <utility>
#include <iostream>
#include <queue>
#include "macros.hpp"

class Client
{
	int							_fd;
	std::pair<std::string, int>	_listen;
	std::string					_remote_address;

	std::string					_request;

	std::queue<std::string*>	_qResponse;
	size_t						_offset;
	bool						_ready;
public:
/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

				Client();
				Client(Client const & src);
				~Client();
	Client &	operator=(Client const & src);

/*----------------------------------METHODS-----------------------------------*/

	void		truncateRequest(const int len);
	void		truncateRequest(const char *end);
	void		setOffset(const ssize_t len);
	void		clearResponse(void);


/*------------------------------GETTERS/SETTERS-------------------------------*/

	void		setFd(const int fd);
	void		setListen(const std::pair<std::string, int>& listen);
	void		setReady(void);

	void		setRemoteAddr(const std::string& remote_addr);

	void		setRequest(const char *request, const int len);

	void		setResponse(std::string *response);


	size_t								getResponseNb(void) const;
	int									getFd(void) const;
	const std::pair<std::string, int>&	getListen(void) const;
	bool								isReady(void) const;
	const std::string&					getRemoteAddr(void) const;

	int									getRequestSize() const;
	const std::string&					getStrRequest(void) const;
	const char*							getRawRequest(void) const;

	size_t								getResponseSize() const;
	const std::string&					getStrResponse(void) const;
	const char*							getRawResponse(void) const;
};

std::ostream &operator<<(std::ostream &out, Client const &value);

#endif
