#include "CgiEnv.hpp"


/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

CgiEnv::CgiEnv(void)
{}

CgiEnv::CgiEnv(CgiEnv const & src) { *this = src; }

CgiEnv::~CgiEnv()
{}

CgiEnv &	CgiEnv::operator=(CgiEnv const & src)
{
	if (this == &src)
		return *this;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/



/*------------------------------GETTERS/SETTERS-------------------------------*/

void	CgiEnv::setMetaVar(const Request& request)
{
	const std::string&	str;

	str = request.getHeader("content-lenght");
}

std::ostream &operator<<(std::ostream &out, CgiEnv const &value)
{
	//out << "I am " << value.getName() << std::endl;
	return (out);
}

