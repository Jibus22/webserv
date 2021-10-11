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
	char			*buf;

	if (!ifs.good())
		return ;
	response.reserve(response.size() + filesize);
	buf = new char [filesize];
	ifs.read(buf, filesize);
	response.append(buf, filesize);
	delete buf;
	ifs.close();
}

int		data_to_file(const std::string& filename, const char* addr, size_t len)
{
	std::ofstream	ofs(filename);

	if (!ofs.good() || !ofs.is_open())
		return -1;
	ofs.write(addr, len);
	ofs.close();
	if (!ofs.good())
		return -1;
	return 0;
}

