#include <sys/types.h>
#include <dirent.h>
#include "webserv.hpp"

void	auto_index(Response & response, std::string const & target)
{
	DIR * dir;
	struct dirent * fic;
	std::string 	prefix;

	if (target[target.size() - 1] != '/')
	{
		prefix = target.substr(target.rfind('/') + 1) + '/';
	}
	response.set_status_code("200");
	response.set_status_infos("OK");
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
		response.append(prefix);
		response.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			response.append("/");

		response.append("\"> ");
		response.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			response.append("/");
		response.append("</a></li>\n");
	}
	response.append("</ul></body></html>");
	std::stringstream ss;
	ss << response.get_body().size();
	response.add_header("Content-Length", ss.str());
}
