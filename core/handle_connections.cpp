#include "webserv.hpp"

//Check if event_fd belongs to server_map, which would means a new client
//wanna connect
int	check_new_connection(const int event_fd,
			const std::map<int, std::pair<std::string, int> >& server_map)
{
	std::map<int, std::pair<std::string, int> >::const_iterator i;

	i = server_map.find(event_fd);
	if (i == server_map.end())
		return -1;
	else
		return i->first;
}

//1st accept new client. 2nd monitor its read events by adding it to kqueue
//3st set Client object & add it to client_map
int	accept_new_client(const int kq, const int event_fd,
			std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	std::map<int, std::pair<std::string, int> >::const_iterator i;
	Client				newclient;
	struct sockaddr_in	sockaddr;
	socklen_t			len = sizeof(sockaddr);
	int					client_fd;

	client_fd = accept(event_fd, (struct sockaddr*) &sockaddr, &len);
	if (check_return(client_fd, client_map) == -1)
		return -1;
	if (add_read_event(kq, client_fd) == -1)
		return -1;

	i = server_map.find(event_fd);
	if (i == server_map.end())
		return pgm_err("accept_new_client() error: client already exists");
	newclient.setFd(client_fd);
	newclient.setListen(i->second);
	newclient.setRemoteAddr(inet_ntoa(sockaddr.sin_addr));
	client_map[client_fd] = newclient;

	__D_DISPLAY(std::endl << "--- server socket event: " << event_fd);
	__D_DISPLAY("New client: " << newclient
			<< "had been accepted on socket " << client_fd
			<< " and is now monitored ---" << std::endl);

	return 0;
}

//Sets all our IP:PORT listening sockets in the kqueue so they are monitored
//for read event
int	monitor_network_sockets(const int kq,
				const std::map<int, std::pair<std::string, int> > & server_map)
{
	for (std::map<int, std::pair<std::string, int> >::const_iterator
			i = server_map.begin(); i != server_map.end(); i++)
	{
		if (add_read_event(kq, i->first))
			return -1;
	}
	return 0;
}
