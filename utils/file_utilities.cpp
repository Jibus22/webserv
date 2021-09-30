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
	ssize_t	ret;
	char	*buf = new char [filesize];
	int		fd = open(filename.c_str(), O_RDONLY);

	if (fd == -1)
		return ;
	ret = read(fd, buf, filesize);
	close(fd);
	if (ret > -1)
		response.append(buf, ret);
	delete [] buf;
}
