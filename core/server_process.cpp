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

//Allocate a buffer with the size available into the buffer from the socket
//connection, receive it and append it to the client requests string
int	read_request(const struct kevent& event, Client& client)
{
	char	*buf;
	ssize_t	len, buflen;

	__D_DISPLAY("data flag from read event: [" << event.data << "]");
	buflen = event.data;
	buf = new char [buflen];
	len = recv(event.ident, buf, buflen, 0);
	__D_DISPLAY_RECV(event.ident, len);
	if (len > 0)
		client.setRequest(buf, len);
	delete [] buf;
	return len;
}

//Send response & set write event to the client again if all response couldn't
//be sent. Removes data that has been sent from the response buffer
int	send_response(const int kq, const struct kevent& event, Client& client)
{
	ssize_t	len;

	//event->data contains space remaining in the write buffer
	__D_DISPLAY("data flag from write event: " << event.data);
	if (event.data < static_cast<long>(client.getLenToSend()))
	{
		len = send(event.ident, client.getRawResponse(), event.data, 0);
		__D_DISPLAY_SEND(client.getFd(), len, 1, client.getStrResponse());
		client.setOffset(len);
		if (len > 0)
			return set_write_ready(kq, client);
	}
	else
	{
		len = send(event.ident, client.getRawResponse(),
				client.getLenToSend(), 0);
		__D_DISPLAY_SEND(client.getFd(), len, 2, client.getStrResponse());
		client.clearResponse();
	}
	return len;
}
