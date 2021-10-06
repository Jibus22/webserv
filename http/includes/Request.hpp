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
	size_t								_blankline;
	const std::string&					_request;
	std::string							_path;

	/*
	void add_header(std::string const & header);
	void parse_first_line(std::string const & first_line);
	void checkTerminatedBody();
	*/

public:

	/*
	** CANONICAL FUNCS
	*/
/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

	//Request(void);
	Request(std::string const & raw_request);
	Request(Request const & src);
	~Request(void);
	Request & operator=(Request const & rhs);



/*------------------------------GETTERS/SETTERS-------------------------------*/

	std::map<std::string, std::string>::const_iterator
						getHeader(const std::string& key_header, bool& found)
							const;
	bool				getHeader(const std::string& key, std::string& value)
							const;
	std::map<std::string, std::string> const &
						get_headers() const;
	std::string const &	get_method() const;
	std::string const & get_target() const;
	std::string const & get_version() const;
	const std::string&	getRequest() const;
	size_t				getBodyPos() const;
	size_t				getBodySize() const;
	const char*			getBodyAddr() const;
	std::string&		getPath();
	std::string const &	getPath() const;

	void				setTarget(const std::string& newtarget);
	void				setPath(const std::string& uri, const std::string& root);


/*------------------------------OVERLOAD OPERATORS----------------------------*/

	std::string const & operator[] (std::string const & key_header);


/*------------------------------EXCEPTIONS------------------------------------*/

	/*
	class NotTerminatedException:std::exception
	{
		const char* what() const throw();
	};

	class InvalidRequest : std::exception
	{
		const char* what() const throw();
	};
	*/
};

std::ostream &operator<<(std::ostream &out, const Request& value);

#endif
