#include "webserv.hpp"

//Set all our IP:PORT listening sockets in the kqueue so they are monitored
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
	if (client_fd == -1)
		return sys_err("failed to accept a new client");
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

//Receive new data, add it to client buffer
//& process it (HTTP parsing + processing)
int	read_request(const int event_fd,
					const std::vector<Server_config*>& server_blocks,
					std::map<int, Client>& client_map)
{
	char	buf[RCV_BUF + 1];
	ssize_t	len;
	Client&	client = client_map[event_fd];

	len = recv(event_fd, buf, RCV_BUF, 0);
	buf[len] = 0;
	client.setRequest(buf, len);

	__D_DISPLAY(std::endl << "Client " << event_fd << ": " << len
			<< " bytes had been read.");

	process_request(client, server_blocks);
	return 0;
}

//Send response & set write event to the client again if all response couldn't
//be sent. Removes data that has been sent from the response buffer
int	send_response(const int kq, const struct kevent *event, Client& client)
{
	int	len;

	//event->data contains space remaining in the write buffer
	__D_DISPLAY("data flag from write event: " << event->data);
	if (event->data < static_cast<long>(client.getResponseSize()))
	{
		len = send(event->ident, client.getRawResponse(), event->data, 0);
		client.truncateResponse(len);
		__D_DISPLAY("1. just sent: " << client.getStrResponse());
		return set_write_ready(kq, client);
	}
	else
	{
		len = send(event->ident, client.getRawResponse(),
				client.getResponseSize(), 0);
		__D_DISPLAY("2. just sent: " << client.getStrResponse());
		client.clearResponse();
	}
	return 0;
}

//Checks if the current client has response to send. If it newly has, sets
//new write event to this client or return 0 to call send_response()
int	is_response(const int kq, const struct kevent *event,
					std::map<int, Client>& client_map)
{ 
	std::map<int, Client>::iterator	i = client_map.find(event->ident);

	if (i == client_map.end())
		return pgm_err("send_request : oops, client should exist");

	if (i->second.getResponseNb() == 0)
		return EMPTY;
	else if (i->second.getResponseNb() > 0 && i->second.isReady() == false)
		return set_write_ready(kq, i->second);
	else if (i->second.isReady() && event->filter == EVFILT_WRITE)
		return 0;
	else
		return -1;
}
