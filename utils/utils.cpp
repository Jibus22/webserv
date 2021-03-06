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
