#include "webserv.hpp"

//Allocate a buffer with the size available into the buffer from the socket
//connection, receive it and append it to the client requests string
int	read_request(const struct epoll_event& event, Client& client)
{
	char	buf[RCV_BUF];
	ssize_t	len;

	len = recv(event.data.fd, buf, RCV_BUF, 0);
	__D_DISPLAY_RECV(event.data.fd, len);
	if (len > 0)
		client.setRequest(buf, len);
	return len;
}

//Send response & set write event to the client again if all response couldn't
//be sent. Removes data that has been sent from the response buffer
int	send_response(const int kq, const struct epoll_event& event, Client& client)
{
	ssize_t	len;
	int		val;
	socklen_t	bla = sizeof(val);

	if (!client.getResponseSize())
	{
		std::cerr << "FUCK ME BITCH" << std::endl;
		return -1;
	}
	getsockopt(client.getFd(), SOL_SOCKET, SO_SNDBUF, &val, &bla);
	__D_DISPLAY("getsockopt send socket: " << val);
	if (val < static_cast<int>(client.getLenToSend()))
	{
		len = send(event.data.fd, client.getRawResponse(), val, 0);
		__D_DISPLAY_SEND(client.getFd(), len, 1, client.getStrResponse());
		client.setOffset(len);
		//if (len > 0)
			//return set_write_ready(kq, client);
	}
	else
	{
		len = send(event.data.fd, client.getRawResponse(),
				client.getLenToSend(), 0);
		__D_DISPLAY_SEND(client.getFd(), len, 2, client.getStrResponse());
		client.clearResponse();
		remove_write_event(kq, client.getFd());
	}
	return len;
}
