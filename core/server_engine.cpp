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
	//While data will be available to read on the socket buffer, we will be
	//triggered. When there isn't any data, no trigger: block.

	i = server_map.find(event_fd);
	if (i == server_map.end())
		return pgm_err("accept_new_client() error: oops");
	newclient.setFd(client_fd);
	newclient.setListen(i->second);
	client_map[client_fd] = newclient;

	__D_DISPLAY(std::endl << "--- server socket event: " << event_fd);
	__D_DISPLAY("New client: " << newclient
			<< "had been accepted on socket " << client_fd
			<< " and is now monitored ---" << std::endl);

	return 0;
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

	__D_DISPLAY(std::endl << "Client " << event_fd << ": " << len
			<< " bytes had been read.");

	process_request(client, server_blocks);
	return 0;
}

static int	set_ready(const int kq, Client& client)
{
	struct kevent					changelist;

	EV_SET(&changelist, client.getFd(), EVFILT_WRITE,
			EV_ADD | EV_ONESHOT, 0, 0, 0);
	if (kevent(kq, &changelist, 1, NULL, 0, NULL) == -1)
		return sys_err("kevent client_fd failed");
	client.setFlag(READY);
	return READY;
}

static int	send_request(const int kq, const struct kevent *event,
					std::map<int, Client>& client_map)
{ 
	int								len;
	std::map<int, Client>::iterator	i = client_map.find(event->ident);

	if (i == client_map.end())
		return pgm_err("send_request : oops, client should exist");

	if (i->second.getFlag() == INCOMPLETE)//response not processed: return
		return INCOMPLETE;
	if (i->second.getFlag() == COMPLETE)//response processed: poll write event
		return set_ready(kq, i->second);

	//Write thru write kevent
	//event->data contains space remaining in the write buffer
	if (i->second.getFlag() == READY && event->filter == EVFILT_WRITE)
	{
		__D_DISPLAY("data flag from write event: " << event->data);
		if (event->data < static_cast<long>(i->second.getRaw().size()))
		{
			len = send(event->ident, i->second.getRaw().c_str(), event->data, 0);
			i->second.truncateRaw(len);
			return set_ready(kq, i->second);
		}
		else
		{
			len = send(event->ident, i->second.getRaw().c_str(),
							i->second.getRaw().size(), 0);
			i->second.setResponse("", INCOMPLETE);
		}
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
				read_request(event_fd, server_blocks, *client_map);
			}
			send_request(kq, &(eventlist[i]), *client_map);
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
