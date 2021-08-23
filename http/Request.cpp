#include "Request.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>

const char* Request::NotTerminatedException::what() const throw()
{
	return "Request not Terminated";
}

const char* Request::InvalidRequest::what() const throw()
{
	return "Invalid Request";
}

std::vector<std::string> & split(const std::string& s, char const separator)
{
	std::vector<std::string> * output = new std::vector<std::string>();
	std::string::size_type prev_pos = 0, pos = 0;

	while((pos = s.find(separator, pos)) != std::string::npos)
	{
		std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output->push_back(substring);
        prev_pos = ++pos;
	}
    output->push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
    return *output;
}



/*
** CANONICAL FUNCS
*/
Request::Request(void){
	return;
}

Request::Request(Request const & src){
	*this = src;
	return;
}

Request &	Request::operator=(Request const & rhs){
	this->_method = rhs._method;
	this->_target = rhs._target;
	this->_headers = rhs._headers;
	this->_body = rhs._body;
	return *this;
}


Request::~Request(void){
	return;
}

void Request::add_header(std::string const header)
{
	size_t pos_separateur = header.find(": ");

	if (pos_separateur == std::string::npos)
		;//TODO: exception
	else
		this->_headers[header.substr(0, pos_separateur)] = header.substr(pos_separateur + 2);
}

void Request::parse_first_line(std::string const first_line)
{
	std::vector<std::string> & words = split(first_line, ' ');

	if(words.size() != 3)
		std::cerr << "erreur taille premiere ligne " <<std::endl;//erreur
	this->_method = words[0];
	this->_target = words[1];
	this->_version = words[2];
}


// TODO:  Exception a gerer si requete mal formate
Request::Request(std::string const & raw_r)
{
	std::string raw_request(raw_r);
	//stocke le body et le supprime de la string
	size_t pos_body = raw_request.find("\n\n");
	if (pos_body == std::string::npos)
	{
		throw Request::NotTerminatedException();
	}
	else
	{
		this->_body = raw_request.substr(pos_body + 2);
		raw_request.erase(pos_body);
	}

	//Separe chaque ligne de la requete
	std::vector<std::string> & lines = split(raw_request, '\n');
	std::vector<std::string>::iterator it = lines.begin();
	std::vector<std::string>::iterator ite = lines.end();

	if(it != ite)
		parse_first_line(*(it++));
	//TODO: else ligne vide -> exception

	while(it != ite)
	{
		this->add_header(*it);
		it++;
	}
}

std::string const & Request::operator[] (std::string const & key_header)
{return this->_headers[key_header];}

std::string const & Request::get_header(const std::string& key_header)
{return this->_headers[key_header];}

std::map<std::string, std::string> const & Request::get_headers() const
{return this->_headers;}

std::string const & Request::get_method() const {return this->_method;}
std::string const & Request::get_target() const {return this->_target;}
std::string const & Request::get_version() const {return this->_version;}
std::string const & Request::get_body() const {return this->_body;}
