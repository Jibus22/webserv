#include "webserv.hpp"

static void	create_serv_vec(std::vector<Server_config *>& srvs,
				const std::string ip, int port, const int nb)
{
	for (int i = 0; i < nb; i++)
	{
		Server_config *newserv = new Server_config();

		newserv->listen.first = ip;
		newserv->listen.second = port++;
		srvs.push_back(newserv);
	}
}

std::vector<Server_config *>	*get_servers_simulation()
{
	std::vector<Server_config *>	*srvs = new std::vector<Server_config *>;

	create_serv_vec(*srvs, "0.0.0.0", 8080, 5);
	//create_serv_vec(*srvs, "192.168.1.120", 8080, 5);
	create_serv_vec(*srvs, "127.0.0.1", 8086, 5);
	__D_DISPLAY("nb of server blocks: " << srvs->size());
	return srvs;
}
