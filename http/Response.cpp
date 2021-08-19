#include "Response.hpp"
#include <iostream>
#include <string>
#include <map>

/*
** CANONICAL FUNCS
*/
Response::Response(void){
	return;
}

Response::Response(Response const & src){
	*this = src;
	return;
}

Response &	Response::operator=(Response const & rhs){
	this->_status_code = rhs._status_code;
	this->_status_infos = rhs._status_infos;
	this->_headers = rhs._headers;
	this->_body = rhs._body;
	return *this;
}

Response::~Response(void){
	return;
}

void Response::set_status_code(int status_code)
{this->_status_code = status_code;}

void Response::set_status_infos(std::string const & status_infos)
{this->_status_infos = status_infos;}

void Response::add_header(std::string const & key_header, std::string const & value)
{this->_headers[key_header] = value;}

void Response::set_body(std::string const & body)
{this->_body = body;}

std::string const Response::get_raw()
{
	std::string raw_response("HTTP/1.1 ");
	raw_response.append(this->_status_code);
	raw_response.append(" ");
	raw_response.append(this->_status_infos);
	raw_response.append("\n");
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
	std::map<std::string, std::string>::iterator ite = this->_headers.end();
	while(it != ite)
	{
		raw_response.append(it->first);
		raw_response.append(": ");
		raw_response.append(it->second);
		raw_response.append("\n");
		it++;
	}
	raw_response.append("\n");
	raw_response.append(this->_body);
	return raw_response;
}
