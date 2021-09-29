#include "webserv.hpp"

//return a timestamp with a precision of 1ms (1s = 1000ms).
long	get_timestamp(const struct timeval& tv_start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec - tv_start.tv_sec) * 1000)
	+ ((now.tv_usec - tv_start.tv_usec) / 1000));
}

int		ft_string_to_nb(const std::string& value)
{
	std::string::const_iterator i = value.begin(), end = value.end();
	std::stringstream				strm(value);
	int							nb = INT_MAX;

	while (i != end)
	{
		if (!isdigit(*i) && !isspace(*i))
			return nb;
		i++;
	}
	strm >> nb;
	return nb;
}

std::string	ft_int_to_string(int val)
{
  std::stringstream	ss;

  ss << val;
  return ss.str();
}

bool	is_file_exist(const char *filename)
{
	return (access(filename, F_OK) == 0);
}

bool	is_file_exist(const std::string& filename)
{
	return (access(filename.c_str(), F_OK) == 0);
}

std::string str_to_lower(std::string const & s)
{
	std::string str(s);
	std::string::iterator it = str.begin();
	std::string::iterator ite = str.end();
	while (it != ite)
	{
		*it = std::tolower(*it);
		it++;
	}
	return str;
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
	char	*buf = new char [filesize];
	int		fd = open(filename.c_str(), O_RDONLY);

	if (fd == -1)
		return ;
	read(fd, buf, filesize);
	response.append(buf);
	delete [] buf;
}
