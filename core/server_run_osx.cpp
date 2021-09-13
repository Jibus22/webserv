#include "webserv.hpp"

//Server engine. System is set to notify any event on any server
//socket descriptors & client socket descriptor which are monitored.
static int	run_server(const int kq,
				const std::vector<Server_config*>& server_blocks,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	int						i, event_fd, new_events;
	struct kevent			eventlist[MAX_EVENTS];
	std::map<int, Client>	*client_map = new std::map<int, Client>;

	for (;;)//Main loop to wait new events
	{
		__D_DISPLAY("waiting new event...");
		new_events = kevent(kq, NULL, 0, eventlist, MAX_EVENTS, NULL);
		if (new_events == -1)
			return sys_err("kevent new event failed");
		for (i = 0; i < new_events; i++)//loop to process triggered events
		{
			event_fd = eventlist[i].ident;
			if (eventlist[i].flags & EV_EOF)
			{
				__D_DISPLAY(" client " << event_fd << " has disconnected");
				close(event_fd);
				client_map->erase(event_fd);
				continue;
			}
			else if (check_new_connection(event_fd, server_map) >= 0)
			{
				if (accept_new_client(kq, event_fd, *client_map, server_map)
						== -1)
					return -1;
				continue;
			}
			else if (eventlist[i].filter == EVFILT_READ)//2.
			{
				read_request(event_fd, *client_map);
				process_request((*client_map)[event_fd], server_blocks,
						*client_map, server_map);
			}
			if (is_response(kq, &(eventlist[i]), *client_map) == 0)
			{
				send_response(kq, &(eventlist[i]), (*client_map)[event_fd]);
			}
		}
	}
	delete client_map;
	return 0;
}

//Set all our IP:PORT listening sockets in the kqueue so they are monitored
//for read event
static int	monitor_network_sockets(const int kq,
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

//Creates kqueue and add server sockets to it so they are monitored
//Then run the server.
int	start_server(const std::vector<Server_config*> & server_blocks,
					std::map<int, std::pair<std::string, int> >	& server_map)
{
	const int	kq = kqueue();

	if (monitor_network_sockets(kq, server_map) == -1)
		return -1;
	if (run_server(kq, server_blocks, server_map) == -1)
		return -1;
	close(kq);
	return 0;
}
