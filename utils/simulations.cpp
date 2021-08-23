#include "webserv.hpp"

//temporary function to simulate all ports given back by the file parsing
void	bunch_of_ports_simulation(std::vector<int>& ports)
{
	int					nb = 8080;

	for (int i = 0; i < 6; i++, nb++)
		ports.push_back(nb);
}

static void	create_serv_vec(std::vector<si_server> srvs,
				const std::string ip, int port, const int nb)
{
	for (int i = 0; i < nb; i++)
	{
		si_server		newserv;

		newserv.listen.first = ip;
		newserv.listen.second = port++;
		srvs.push_back(newserv);
	}
}

std::vector<si_server>	*get_servers_simulation()
{
	std::vector<si_server>	*srvs = new std::vector<si_server>;

	create_serv_vec(*srvs, "0.0.0.0", 8080, 5);
	create_serv_vec(*srvs, "0.0.0.0", 8083, 5);
	create_serv_vec(*srvs, "127.0.0.1", 8080, 5);
	if (_DEBUG)
	{
		__D_DISPLAY("nb of server blocks: " << srvs->size());
	}
	return srvs;
}
