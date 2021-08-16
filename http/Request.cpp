#include "Request.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>

std::vector<std::string> & split(const std::string& s, char separator)
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
/*Request::Request(void){
	return;
}

Request::Request(Request const & src){
	*this = src;
	return;
}

Request &	Request::operator=(Request const & rhs){
	return *this;
}
*/

Request::~Request(void){
	return;
}

void Request::add_header(std::string header)
{
	size_t pos_separateur = header.find(": ");

	if (pos_separateur == std::string::npos)
		;//TODO: exception
	else
		this->_headers[header.substr(0, pos_separateur)] = header.substr(pos_separateur + 2);
}

void Request::parse_first_line(std::string first_line)
{
	std::vector<std::string> & words = split(first_line, ' ');

	if(words.size() != 3)
		std::cerr << "erreur taille premiere ligne " <<std::endl;//erreur
	this->_method = words[0];
	this->_target = words[1];
	this->_version = words[2];
}


// TODO:  Exception a gerer si requete mal formate
Request::Request(std::string & raw_request)
{
	//stocke le body et le supprime de la string
	size_t pos_body = raw_request.find("\n\n");
	if (pos_body != std::string::npos)
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
	//else ligne vide -> exception

	while(it != ite)
	{
		this->add_header(*it);
		it++;
	}
}

std::string& Request::operator[] (const std::string& key_header)
{return this->_headers[key_header];}

std::string& Request::get_header(const std::string& key_header)
{return this->_headers[key_header];}

std::map<std::string, std::string> Request::get_headers()
{return this->_headers;}

std::string& Request::get_method(){return this->_method;}
std::string& Request::get_target(){return this->_target;}
std::string& Request::get_version(){return this->_version;}
std::string& Request::get_body(){return this->_body;}
