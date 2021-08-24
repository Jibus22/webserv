#include "webserv.hpp"

static void	create_serv_vec(std::vector<SiServ>& srvs,
				const std::string ip, int port, const int nb)
{
	for (int i = 0; i < nb; i++)
	{
		SiServ		newserv;

		newserv.listen.first = ip;
		newserv.listen.second = port++;
		srvs.push_back(newserv);
	}
}

std::vector<SiServ>	*get_servers_simulation()
{
	std::vector<SiServ>	*srvs = new std::vector<SiServ>;

	create_serv_vec(*srvs, "0.0.0.0", 8080, 5);
	//create_serv_vec(*srvs, "192.168.1.120", 8080, 5);
	create_serv_vec(*srvs, "127.0.0.1", 8086, 5);
	__D_DISPLAY("nb of server blocks: " << srvs->size());
	return srvs;
}
