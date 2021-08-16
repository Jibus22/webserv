#ifndef REQUEST_H
# define REQUEST_H

#include <iostream>
#include <string>
#include <map>

class Request{

private:
	std::string							_method;
	std::string 						_target;
	std::string							_version;
	std::map<std::string, std::string>	_headers;
	std::string							_body;


	void add_header(std::string header);
	void parse_first_line(std::string first_line);


public:

	/*
	** CANONICAL FUNCS
	*/
	//Request(void);
	//Request(Request const & src);
	~Request(void);
	//Request & operator=(Request const & rhs);

	Request(std::string & raw_request);

	std::string& operator[] (const std::string& key_header);
	std::string& get_header(const std::string& key_header);
	std::map<std::string, std::string> get_headers();
	std::string& get_method();
	std::string& get_target();
	std::string& get_version();
	std::string& get_body();
};

#endif
