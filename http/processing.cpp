#include "processing.hpp"

void	process_request(Client& client,
				const std::vector<Server_config*>& server_blocks)
{
	Response	test;
	Request		test2;
	const char	*request = client.getRawRequest();
	std::string	*response;

	__D_DISPLAY(client);

	for (int i = 0; i < client.getRequestSize(); i++)
	{
		if (*request == 'x')
		{
			response = new std::string(client.getRawRequest(), request + 1);
			response->insert(0, "Response: ");
			client.truncateRequest(request + 1);
			client.setResponse(response);

			//--------------//
			__D_DISPLAY("X had been found into the request!");
			__D_DISPLAY("Truncated request: |" << client.getStrRequest() << "|");
			__D_DISPLAY("Response: |" << client.getStrResponse() << "|");
		}
		request++;
	}

	(void)server_blocks;
	(void)test;
	(void)test2;
}
