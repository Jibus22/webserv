#ifndef CGIENV_HPP
#define CGIENV_HPP

#include "Request.hpp"
#include "CgiMetaVar.hpp"
#include <string>
#include <vector>

class CgiEnv
{
	CgiMetaVar					_metaVar;
	std::vector<std::string*>	_env;
public:
	CgiEnv();
	CgiEnv(const CgiEnv& src);
	~CgiEnv();
	CgiEnv&	operator=(const CgiEnv& src);

	void	setMetaVar(const Request& request);
};

std::ostream &operator<<(std::ostream &out, CgiEnv const &value);

#endif
