#include "webserv.hpp"

std::string	auto_index(const std::string& dir_name, const std::string& target)
{
	DIR				*dir;
	struct dirent	*fic;
	std::string		value;

	dir = opendir(dir_name.c_str());
	if (!dir)
		return "";
	value.assign("<html>\n<head>\n<meta charset=\"utf-8\">\n"
			"<title>Directory Listing</title>\n</head>\n<body>\n<h1>"
			+ dir_name + "</h1>\n<ul>");
	while ((fic = readdir(dir)) != NULL)
	{
		value.append("<li><a href=\"");
		value.append(target);
		if (value[value.size() - 1] != '/')
			value.append("/");
		value.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			value.append("/");
		value.append("\"> ");
		value.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			value.append("/");
		value.append("</a></li>\n");
	}
	value.append("</ul></body></html>");
	closedir(dir);
	return value;
}
