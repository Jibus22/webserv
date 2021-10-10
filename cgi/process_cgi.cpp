#include "webserv.hpp"

int		cgi_exit_status(const int& status)
{
	int	ret = 0;

	if (WIFEXITED(status))
	{
		ret = WEXITSTATUS(status); 
		if (ret == EXIT_FAILURE)
		{__D_DISPLAY("Oops, cgi exited with error: " << ret);}
		else if (ret == EXIT_SUCCESS)
		{__D_DISPLAY("CGI SUCCESS: " << ret << ", we can process response.");}
		else
		{__D_DISPLAY("Cgi exited with status: " << WEXITSTATUS(status));}
	}
	else if (WIFSIGNALED(status))
	{
		ret = WTERMSIG(status);
		__D_DISPLAY("Cgi exited with signal: " << ret);
	}
	__D_DISPLAY("PARENT: finished to wait for cgi");
	return ret;
}

bool	is_child_slow(const struct timeval& tv, const pid_t c_pid)
{
	if (get_timestamp(tv) > CGI_TIME_LIMIT)
	{
		kill(c_pid, SIGTERM);
		usleep(1000);
		return true;
	}
	return false;
}

//WNOHANG flag to waitpid() permits us to read pipe in real time while the child
//is writing into it. It permits to avoid either a blocking write from the
//script side, or a loss of data.
int		read_cgi_output(const pid_t c_pid, const FtPipe& rx,
					std::string& cgi_out)
{
	struct timeval	tv;
	pid_t			w_pid;
	int				status, ret = 1;
	char			buf[CGI_RD_BUF_LEN];

	if (gettimeofday(&tv, NULL) == -1)
		return EXIT_FAILURE;
	while ((w_pid = waitpid(c_pid, &status, WNOHANG)) == 0)
	{
		if (is_child_slow(tv, c_pid))
			break ;
		while (ret > 0)
		{
			if (is_child_slow(tv, c_pid))
				break ;
			ret = read(rx.read, buf, CGI_RD_BUF_LEN - 1);
			if (ret == -1)
			{
				close(rx.read);
				return EXIT_FAILURE;
			}
			cgi_out.append(buf, ret);
		}
	}
	close(rx.read);
	if (w_pid == -1)
		return EXIT_FAILURE;
	return cgi_exit_status(status);
}

//macosx: PIPE_BUF == 512
//This is the POSIX length of a pipe message which is guaranteed to not be mixed
//with other write operation from other threads. It is called 'atomic write'
//but I don't use it here. hohoho.
int		write_to_child(const Request& request, const FtPipe& tx)
{
	ssize_t	ret;

	if (request.getBodySize() == 0)
		return 0;
	ret = write(tx.write, request.getBodyAddr(), request.getBodySize());
	close(tx.write);
	if (ret == -1 || ret == 0)
		return 1;
	return 0;
}

//int execve(const char *pathname, char *const argv[], char *const envp[]);
//pathname must be formated as: /absolutepath/name. Ex: "/bin/ls"
//argv[0] must hold [name | path/name]. Ex: ["ls" | "bin/ls"]
//argv[n] holds arguments to the bin arguments. Ex: "-la"
//argv[end] must be NULL.
void	exec_cgi_script(CgiEnv& env, FtPipe& rx, FtPipe& tx,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	int	ret;

	close_server_sockets(server_map);
	close_client_sockets(client_map);
	tx.hijackStdinReadPipe();
	rx.hijackStdoutWritePipe();
	//usleep(500);//wait for the parent to write into tx.write pipe
	ret = execve((env.getArgv())[0], env.getArgv(), env.getEnv());
	if (ret == -1)
	{
		//perror("execve");
		exit(EXIT_FAILURE);
	}
}

//If an http body exist, send it to the script.
//read the script output & parse it. The output can be a document to send back
//or an absolute URI. If it is a relative URI, reprocess it.
int		write_read_cgi(FtPipe& rx, FtPipe& tx, const int c_pid, Client& client,
				Request& request, const Server_config& server_block)
{
	std::string	*cgi_out;
	int			cgi_exit, cgi_status = CGI_SUCCESS;

	close(tx.read);
	close(rx.write);
	if (write_to_child(request, tx))
		return http_error(client, server_block.error_page, 500, cgi_status);
	cgi_out = new std::string();
	cgi_exit = read_cgi_output(c_pid, rx, *cgi_out);
	__D_DISPLAY("CGI_OUT:\n" << *cgi_out);
	if (cgi_exit == EXIT_SUCCESS)
		cgi_status = cgi_output(*cgi_out);
	else
		cgi_status = CGI_ERR;
	if (cgi_status == CGI_ERR)
	{
		delete cgi_out;
		return http_error(client, server_block.error_page, 500, cgi_status);
	}
	else if (cgi_status == CGI_REDIRECT)
	{
		request.setTarget(*cgi_out);
		delete cgi_out;
		return cgi_status;
	}
	client.setResponse(cgi_out);
	client.clearRequest();
	return cgi_status;
}


int		process_cgi(Request& request, const Location_config& location_block,
				const Server_config& server_block, Client& client,
				const std::string& cgi_ext,
				const std::map<int, Client>& client_map,
				const std::map<int, std::pair<std::string, int> >& server_map)
{
	CgiEnv		env(request, location_block, server_block, client, cgi_ext);
	pid_t		c_pid;
	int			ret = 0;
	FtPipe		tx, rx;

	__D_DISPLAY("ENV:\n" << env);
	if (!is_file_exist(((env.getArgs())[1]).c_str()))
		return http_error(client, server_block.error_page, 404, 2);
	if (tx.isPipeError() || rx.isPipeError())
		return http_error(client, server_block.error_page, 500, 2);
	c_pid = fork();
	if (c_pid == -1)
		return http_error(client, server_block.error_page, 500, 2);
	else if (c_pid == 0)//child
	{
		exec_cgi_script(env, rx, tx, client_map, server_map);
	}
	else
	{
		ret = write_read_cgi(rx, tx, c_pid, client, request, server_block);
	}
	return ret;
}
