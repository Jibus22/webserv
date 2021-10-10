#include "webserv.hpp"

//Server engine. System is set to notify any event on any server
//socket descriptors & client socket descriptor which are monitored.
static int	run_server(const int kq,
				const std::vector<Server_config*>& server_blocks,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	int						i, event_fd, new_events, ret = 0;
	struct epoll_event		eventlist[MAX_EVENTS];
	std::map<int, Client>	*client_map = new std::map<int, Client>;

	for (;;)//Main loop to wait new events
	{
		__D_DISPLAY(WHITE_C << "\t\t\t\t....waiting new event...." << RESET_C);
		errno = 0;
		new_events = epoll_wait(kq, eventlist, MAX_EVENTS, -1);
		__D_DISPLAY(WHITE_C << "\t\t\t\tnew events: " << new_events << RESET_C);
		if (new_events == -1)
			return sys_err("kevent new event failed");
		for (i = 0; i < new_events; i++)//loop to process triggered events
		{
			event_fd = eventlist[i].data.fd;
			if (eventlist[i].events & EPOLLRDHUP
					|| eventlist[i].events & EPOLLHUP
					|| eventlist[i].events & EPOLLERR)
				remove_client(*client_map, event_fd, kq);
			else if (check_new_connection(event_fd, server_map) >= 0)
			{
				ret = accept_new_client(kq, event_fd, *client_map, server_map);
				if (ret == -1)
					return -1;
			}
			else if (eventlist[i].events & EPOLLIN)//2.
			{
				Client&	client = (*client_map)[event_fd];
				ret = read_request(eventlist[i], client);
				if (ret == -1 || ret == 0)
					remove_client(*client_map, event_fd, kq);
				else
				{
					ret = is_valid_request(client);
					if (ret == VALID_REQUEST)
						process_request(client, server_blocks,
								*client_map, server_map);
					if (ret != INCOMPLETE_REQUEST)
						set_write_ready(kq, client);
				}
			}
			else if (eventlist[i].events & EPOLLOUT)//2.
			{
				ret = send_response(kq, eventlist[i], (*client_map)[event_fd]);
				if (ret == -1 || ret == 0)
					remove_client(*client_map, event_fd, kq);
			}
		}
	}
	delete client_map;
	return 0;
}

//Creates kqueue and add server sockets to it so they are monitored
//Then run the server.
int	start_server(const std::vector<Server_config*>& server_blocks,
					std::map<int, std::pair<std::string, int> >& server_map)
{
	const int	kq = epoll_create(1);

	__DISPLAY_SERVERMAP(server_map);
	if (kq == -1)
		return -1;
	if (monitor_network_sockets(kq, server_map) == -1)
		return -1;
	if (run_server(kq, server_blocks, server_map) == -1)
		return -1;
	close(kq);
	return 0;
}
