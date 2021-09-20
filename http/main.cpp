#include <stdio.h>
#include <string.h>
#include "Request.hpp"
#include "Response.hpp"
#include <map>


int main() {
	std::string raw_request("GET /index.html HTTP/1.1\n\
Host: google.com\n\
TestHeadeRfs:Valeurf\
\r\n\r\n");
	Request r = Request(raw_request);
	std::cout << "Methode : " << r.get_method() << std::endl;
	std::cout << "Target  : " << r.get_target() << std::endl;
	std::cout << "version : " << r.get_version() << std::endl;
	std::cout << "Headers : " << std::endl;
	std::map<std::string, std::string> headers = r.get_headers();
	for(std::map<std::string, std::string>::iterator it = headers.begin();
		it != headers.end(); it++)
		std::cout << "\t|" << it->first << "|: |" <<it->second<<'|'<<std::endl;
	std::cout << "Body    : " << r.get_body() << std::endl;
	return 0;
}
