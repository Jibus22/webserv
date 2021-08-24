#include "webserv.hpp"

//Set all our IP:PORT listening sockets in the kqueue so they are monitored
//for read event
static int	monitor_network_sockets(const int & kq,
				const std::map<int, std::pair<std::string, int> > & server_map)
{
	struct kevent	changelist;

	for (std::map<int, std::pair<std::string, int> >::const_iterator
			i = server_map.begin(); i != server_map.end(); i++)
	{
		EV_SET(&changelist, i->first, EVFILT_READ, EV_ADD, 0, 0, 0);
		if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
			return sys_err("network socks kevent failed");
	}
	return 0;
}

//check if 'event_fd == one of network fd from vector', which would means
//a new client wanna connect.
static int	check_new_connection(const int event_fd,
			const std::map<int, std::pair<std::string, int> >& server_map)
{
	std::map<int, std::pair<std::string, int> >::const_iterator i;

	i = server_map.find(event_fd);
	if (i == server_map.end())
		return -1;
	else
		return i->first;
}

static int	accept_new_client(const int kq, const int event_fd,
				std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	std::map<int, std::pair<std::string, int> >::const_iterator i;
	struct kevent	changelist;
	Client			newclient;
	const int		client_fd = accept(event_fd, NULL, NULL);

	if (client_fd == -1)
		return sys_err("failed to accept a new client");
	EV_SET(&changelist, client_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
	if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
		return sys_err("kevent client_fd failed");

	i = server_map.find(event_fd);
	if (i == server_map.end())
		return pgm_err("accept_new_client() error: oops");
	newclient.setFd(client_fd);
	newclient.setListen(i->second);
	client_map[client_fd] = newclient;

	__D_DISPLAY("event_fd  : " << event_fd);
	__D_DISPLAY("new client: " << newclient);
	__D_DISPLAY(" new client had been accepted on socket "
		<< client_fd << " and is now monitored");

	return 0;
}

void	process_request(Client& client,
				const std::vector<SiServ>& server_blocks)
{
	__D_DISPLAY(client);
	(void)server_blocks;
}

static int	read_request(const int event_fd,
					const std::vector<SiServ>& server_blocks,
					std::map<int, Client>& client_map)
{
	char		buf[RCV_BUF + 1];
	ssize_t		len;
	Client&		client = client_map[event_fd];

	len = recv(event_fd, buf, RCV_BUF, 0);
	buf[len] = 0;
	client.setRaw(buf);

	__D_DISPLAY(" client " << event_fd << ": " << len
			<< " bytes had been read");

	process_request(client, server_blocks);
	if (client.getFlag() == COMPLETE)
	{
		//send; (j'ai des trucs à voir avant. peu-être kevent() WRITE event...)
		client.eraseRaw();
	}
	return 0;
}

int	run_server(const int kq, const std::vector<SiServ> & server_blocks,
				const std::map<int, std::pair<std::string, int> > & server_map)
{
	int						i, event_fd, new_events;
	struct kevent			eventlist[32];
	std::map<int, Client>	*client_map = new std::map<int, Client>;

	for (;;)//Main loop to wait new events
	{
		__D_DISPLAY("waiting new event...");
		new_events = kevent(kq, NULL, 0, eventlist, 32, NULL);
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
			}
			else if (check_new_connection(event_fd, server_map) >= 0)
			{
				if (accept_new_client(kq, event_fd, *client_map, server_map)
						== -1)
					return -1;
			}
			else if (eventlist[i].filter & EVFILT_READ)//2.
			{
				read_request(event_fd, server_blocks, *client_map);
			}
		}
	}
	delete client_map;
}

//Server engine. System is set to notify any event on any network
//socket descriptors & client connection descriptor
int	start_server(const std::vector<SiServ> & server_blocks,
					std::map<int, std::pair<std::string, int> >	& server_map)
{
	const int		kq = kqueue();

	if (monitor_network_sockets(kq, server_map) == -1)
		return -1;
	if (run_server(kq, server_blocks, server_map) == -1)
		return -1;
	close(kq);
	return 0;
}
