#ifndef SIMULATION_HPP
#define SIMULATION_HPP

class si_server
{
	public:
		si_server() {};
		~si_server() {};
		std::pair<std::string, int>		listen;
};

void					bunch_of_ports_simulation(std::vector<int>& ports);
std::vector<si_server>	*get_servers_simulation();
