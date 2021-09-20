#ifndef FTPIPE_HPP
#define FTPIPE_HPP

#include "webserv.hpp"

class FtPipe
{
	int			_fds[2];
	int			_err;

public:
	const int&	read;
	const int&	write;

	FtPipe();
	FtPipe(FtPipe const & src);
	~FtPipe();
	FtPipe&	operator=(const FtPipe& src);

	void	hijackStdoutWritePipe();
	void	hijackStdinReadPipe();

	bool	isPipeError() const;
	int		getErrno() const;

private:
	void	cleanFds();
	bool	isPiped() const;
};

std::ostream &operator<<(std::ostream &out, const FtPipe& value);

#endif

