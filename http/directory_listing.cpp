#include <sys/types.h>
#include <dirent.h>
#include "webserv.hpp"

void	auto_index(Response & response, std::string const & target)
{
	response.set_status_code("200");
	response.set_status_infos("OK");
	DIR * dir;
	struct dirent * fic;

	dir = opendir(target.c_str());
	response.append("\
	<html>\n\
		<head>\n\
			<meta charset=\"utf-8\">\n\
			<title>Directory Listing</title>\n\
		</head>\n\
	<body>\n\
		<h1>");
	response.append(target);
	response.append("</h1>\n<ul>");
	while ((fic = readdir(dir)) != NULL)
	{
		response.append("<li><a href=\"");
		response.append(fic->d_name);
		response.append("\"> ");
		response.append(fic->d_name);
		response.append("</a></li>\n");
	}
	response.append("</ul></body></html>");
}
