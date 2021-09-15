#include "webserv.hpp"

#define PIPE_BUFTMP 16//temporary, for testing

#define CGI_TIME_LIMIT 500 //to set in macros.hpp later. time in ms.

#define WRITE_BUF 10

int		cgi_exit_status(const int& status)
{
	int	ret = 0;

	if (WIFEXITED(status))//option nohang avec un while, pour timestamp?
	{
		ret = WEXITSTATUS(status); 
		if (ret == EXIT_FAILURE)
			std::cerr << "Oops, cgi exited with error: " << ret << std::endl;
		else if (ret == EXIT_SUCCESS)
		{
			__D_DISPLAY("CGI SUCCESS: " << ret << ", we can process response.");
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
	char			buf[PIPE_BUFTMP];

	if (gettimeofday(&tv, NULL) == -1)
		return pgm_perr("gettimeofday");
	while ((w_pid = waitpid(c_pid, &status, WNOHANG)) == 0)
	{
		if (is_child_slow(tv, c_pid))
			break ;
		while (ret > 0)
		{
			__D_DISPLAY("into read");
			if (is_child_slow(tv, c_pid))
				break ;
			ret = read(rx.read, buf, PIPE_BUFTMP - 1);
			if (ret == -1)
				return pgm_perr("read");
			buf[ret] = 0;
			cgi_out.append(buf);
		}
	}
	close(rx.read);
	if (w_pid == -1)
		return pgm_perr("waitpid");
	return cgi_exit_status(status);
}

//macosx: PIPE_BUF == 512
//This is the POSIX length of a pipe message which is guaranteed to not be mixed
//with other write operation from other threads. It is called 'atomic write'
int		write_to_child(const std::string& body, const FtPipe& tx)
{
	std::string::const_pointer	buf;
	ssize_t						ret = 1;

	if (body.empty())
		return 1;
	__D_DISPLAY("WRITETOCHILD body: " << body);
	buf = body.data();
	while (ret > 0)
	{
		__D_DISPLAY("into write");
		ret = write(tx.write, buf, WRITE_BUF);
		if (ret == -1)
			return pgm_perr("write");
		buf += ret;
	}
	close(tx.write);
	return 0;
}

int		process_output()
{
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
	CgiEnv		env(request, location_block, server_block, client, cgi_ext);
	std::string	*cgi_out = new std::string();
	pid_t		c_pid;
	int			ret;
	FtPipe		tx, rx;

	(void)response;
	__D_DISPLAY(env);

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
		//usleep(500);//wait for the parent to write into tx.write pipe
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
		write_to_child(request.get_body(), tx);
		//si il y a un body, l'écrire dans le pipe puis close tx.write
		ret = read_cgi_output(c_pid, rx, *cgi_out);
		__D_DISPLAY("CGI_OUT:\n" << *cgi_out);
		if (ret == EXIT_SUCCESS)
			process_output();
	}
	delete cgi_out;
	return 0;
}
//int execve(const char *pathname, char *const argv[], char *const envp[]);

//pathname must be formated as: /absolutepath/name. Ex: "/bin/ls"
//argv[0] must hold [name | path/name]. Ex: ["ls" | "bin/ls"]
//argv[n] holds arguments to the bin arguments. Ex: "-la"
//argv[end] must be NULL.
