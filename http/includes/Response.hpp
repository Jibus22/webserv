#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <string>
# include <map>

class Response{

private:
	std::string							_status_code;
	std::string							_status_infos;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	bool								_body_path;

public:

	/*
	** CANONICAL FUNCS
	*/
	Response(void);
	Response(Response const & src);
	~Response(void);
	Response & operator=(Response const & rhs);

	void set_status_code(std::string const status_code);
	void set_status_infos(std::string const & status_infos);
	void add_header(std::string const & key_header, std::string const & value);
	void set_body(std::string const & body);
	void set_body_path(std::string const & body_path);
	std::string const & get_body();
	void append(std::string const & body);
//	std::string const * get_raw();
	std::string * get_raw();
};

#endif
