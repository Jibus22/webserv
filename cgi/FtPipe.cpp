#include "FtPipe.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

FtPipe::FtPipe(void) : _err(-1), read(_fds[0]), write(_fds[1])
{
	cleanFds();
	if (pipe(_fds) == -1)
		_err = errno;
}

FtPipe::FtPipe(const FtPipe& src) : _err(-1), read(_fds[0]), write(_fds[1])
{
	_fds[0] = src._fds[0];
	_fds[1] = src._fds[1];
}

FtPipe::~FtPipe()
{}

FtPipe &	FtPipe::operator=(const FtPipe& src)
{
	if (this == &src)
		return *this;
	_fds[0] = src._fds[0];
	_fds[1] = src._fds[1];
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/

void	FtPipe::cleanFds()
{
	memset(_fds, 0, sizeof(int) * 2);
}

void	FtPipe::hijackStdoutWritePipe()
{
	if (isPiped() == false)
		return ;
	dup2(write, STDOUT_FILENO);
	close(read);
	close(write);
	cleanFds();
}

void	FtPipe::hijackStdinReadPipe()
{
	if (isPiped() == false)
		return ;
	dup2(read, STDIN_FILENO);
	close(read);
	close(write);
	cleanFds();
}


/*------------------------------GETTERS/SETTERS-------------------------------*/

bool	FtPipe::isPiped() const {return (_fds[0] != 0 && _fds[1] != 0);};

bool	FtPipe::isPipeError() const {return _err > -1;};

int		FtPipe::getErrno() const {return _err;};


/*------------------------------OVERLOAD OPERATORS----------------------------*/

std::ostream &operator<<(std::ostream &out, const FtPipe& value)
{
	out << "read: " << value.read << "\nwrite: " << value.write << std::endl;
	return (out);
}

