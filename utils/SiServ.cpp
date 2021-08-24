#include "includes/SiServ.hpp"

/*------------------------CONSTRUCTOR / DESTRUCTOR----------------------------*/

SiServ::SiServ(void)
{}

SiServ::SiServ(SiServ const & src) { *this = src; }

SiServ::~SiServ()
{}

SiServ &	SiServ::operator=(SiServ const & src)
{
	if (this == &src)
		return *this;
	listen = src.listen;
	return *this;
}


/*----------------------------------METHODS-----------------------------------*/



/*------------------------------GETTERS/SETTERS-------------------------------*/
