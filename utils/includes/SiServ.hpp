#ifndef SISERV_HPP
# define SISERV_HPP

#include <utility>
#include <string>

class SiServ
{
public:
	SiServ();
	SiServ(const SiServ& src);
	~SiServ();
	SiServ&	operator=(const SiServ& src);

	std::pair<std::string, int>		listen;
};

#endif
