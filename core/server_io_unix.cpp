#include "webserv.hpp"

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
