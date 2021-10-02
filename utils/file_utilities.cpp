#include "webserv.hpp"

bool	is_dir(const std::string name)
{
	struct stat	st;

	if (stat(name.c_str(), &st) == -1)
		return false;
	return S_ISDIR(st.st_mode);
}

bool	is_openable(const std::string& name)
{
	return (access(name.c_str(), R_OK | W_OK) == 0);
}

bool	is_file_exist(const char *filename)
{
	return (access(filename, F_OK) == 0);
}

bool	is_file_exist(const std::string& filename)
{
	return (access(filename.c_str(), F_OK) == 0);
}

int		get_file_size(const char *file)
{
	struct stat	buf;

	if (stat(file, &buf) == -1)
		return -1;
	return buf.st_size;
}

void	file_to_string(std::string& response,
				const std::string& filename, int filesize)
{
	std::ifstream	ifs(filename);

	if (!ifs.good())
		return ;
	response.reserve(response.size() + filesize);
	response.append(std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>());
	ifs.close();
}
