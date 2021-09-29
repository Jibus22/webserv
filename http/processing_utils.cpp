#include <iostream>
#include <vector>
#include <dirent.h>
#include "webserv.hpp"

bool is_dir(const std::string path)
{
	DIR *dir;

    if ((dir = opendir(path.c_str())) != nullptr) {
        closedir (dir);
		return 1;
    }
	else
		return false;
}

bool	is_openable(std::string const & path)
{
	std::ifstream		file;

	if (is_dir(path))
		return false;
	file.open(path.c_str());
	if (file.fail() == true)
		return false;
	return true;
}

bool	get_file_content(std::string const & path, std::string & content)
{
	__D_DISPLAY("target : " << path);
	std::ifstream		file;
	std::string			line;

	if (is_dir(path))
		return false;
	file.open(path.c_str());
	if (file.fail() == true)
		return false;
	while (std::getline(file, line))
		content.append(line + "\n");
	return true;
}
