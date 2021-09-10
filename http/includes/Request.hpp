#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>

class Request{

private:
	std::string							_method;
	std::string							_target;
	std::string							_version;
	std::map<std::string, std::string>	_headers;
	std::string							_body;


	void add_header(std::string const header);
	void parse_first_line(std::string const first_line);
	void checkTerminatedBody();

public:

	/*
	** CANONICAL FUNCS
	*/
	Request(void);
	Request(Request const & src);
	~Request(void);
	Request & operator=(Request const & rhs);

	Request(std::string const & raw_request);

	std::string const & operator[] (std::string const & key_header);
	std::map<std::string, std::string>::const_iterator
		get_header(const std::string& key_header, bool& found) const;
	std::map<std::string, std::string> const & get_headers() const;
	std::string const & get_method() const;
	std::string const & get_target() const;
	std::string const & get_version() const;
	std::string const & get_body() const;

	class NotTerminatedException:std::exception
	{
		const char* what() const throw();
	};

	class InvalidRequest : std::exception
	{
		const char* what() const throw();
	};


};

#endif
