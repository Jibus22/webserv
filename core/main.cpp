#include "webserv.hpp"

//temporary function to simulate all ports given back by the file parsing
std::vector<int>	bunch_of_ports_simulation()
{
	std::vector<int>	ports;
	int					nb = 8080;

	for (int i = 0; i < 6; i++, nb++)
		ports.push_back(nb);
	return ports;
}

int	main(void)
{
	std::vector<int>	ports(bunch_of_ports_simulation());
	std::vector<int>	network_sockets;

	if (create_network_sockets(ports, network_sockets) == -1)
		return -1;
	run_darwin_server(network_sockets);
	close_listening_ports(network_sockets, 0);

	return 0;
}
