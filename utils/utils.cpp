#include "webserv.hpp"

//return a timestamp with a precision of 1ms (1s = 1000ms).
long	get_timestamp(const struct timeval& tv_start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec - tv_start.tv_sec) * 1000)
	+ ((now.tv_usec - tv_start.tv_usec) / 1000));
}