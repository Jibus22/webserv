#ifndef RESPONSE_H
# define RESPONSE_H

#include <iostream>
#include <string>
# include <map>

class Response{

private:
	std::string						_status_code;
	std::string						_status_infos;
	std::map<std::string, std::string>	_headers;
	std::string						_body;


public:

	/*
	** CANONICAL FUNCS
	*/
	Response(void);
	//Response(Response const & src);
	~Response(void);
	//Response & operator=(Response const & rhs);
	void set_status_code(int status_code);
	void set_status_infos(std::string status_infos);
	void add_header(std::string header, std::string value);
	void set_body(std::string & body);
	std::string get_raw();
};

#endif
