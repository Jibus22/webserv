#include "webserv.hpp"

int		wait_cgi_status(const pid_t c_pid)
{
	pid_t	w_pid;
	int		status, ret = 0;

	w_pid = waitpid(c_pid, &status, 0);
	if (WIFEXITED(status))
	{
		ret = WEXITSTATUS(status); 
		if (ret == EXIT_FAILURE)
			std::cerr << "Oops, cgi exited with error: " << ret << std::endl;
		else if (ret == EXIT_SUCCESS)
		{
			__D_DISPLAY("CGI SUCCESS: " << ret << ", read & process response.");
		}
		else
			std::cout << "Cgi exited with status: "
				<< WEXITSTATUS(status) << std::endl;
	}
	else if (WIFSIGNALED(status))
	{
		ret = WTERMSIG(status);
		std::cerr << "Cgi exited with signal: " << ret << std::endl;
	}
	__D_DISPLAY("PARENT: finished to wait for cgi");
	return ret;
}

int		read_cgi_output(const FtPipe& rx)
{
	char	buf[801];
	int		ret;

	ret = read(rx.read, buf, 800);
	buf[ret] = 0;
	std::cout << "READ BUF:\n" << buf << std::endl;
	return 0;
}

int		process_cgi(Response & response,
				const Request& request,
				const Location_config& location_block,
				const Server_config& server_block,
				const Client& client,
				const std::string& cgi_ext,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	pid_t	c_pid;
	int		ret;
	CgiEnv	env(request, location_block, server_block, client, cgi_ext);
	FtPipe	tx, rx;
	(void)response;

	__D_DISPLAY(env);

	//créer des pipes
	if (tx.isPipeError() || rx.isPipeError())
		return pgm_perr("pipe");
	c_pid = fork();
	if (c_pid == -1)
		return pgm_perr("fork");
	else if (c_pid == 0)//child
	{
		close_server_sockets(server_map);
		close_client_sockets(client_map);
		tx.hijackStdinReadPipe();
		rx.hijackStdoutWritePipe();
		ret = execve((env.getArgv())[0], env.getArgv(), env.getEnv());
		if (ret == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		__D_DISPLAY("iNtO fOrK AfTer eXeCvE & ExIt, sHouLDnT bE prInTEd");
	}
	else
	{
		close(tx.read);
		close(rx.write);
		ret = wait_cgi_status(c_pid);
		if (ret == EXIT_SUCCESS)
			read_cgi_output(rx);
	}
	return 0;
}
//int execve(const char *pathname, char *const argv[], char *const envp[]);

//pathname must be formated as: /absolutepath/name. Ex: "/bin/ls"
//argv[0] must hold [name | path/name]. Ex: ["ls" | "bin/ls"]
//argv[n] holds arguments to the bin arguments. Ex: "-la"
//argv[end] must be NULL.
