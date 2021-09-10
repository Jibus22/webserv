#include "webserv.hpp"

int		wait_cgi_status(const pid_t c_pid)
{
	pid_t	w_pid;
	int		status;

	w_pid = waitpid(c_pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == EXIT_FAILURE)
			std::cerr << "Oops, cgi exited with error: "
				<< WEXITSTATUS(status) << std::endl;
		else if (WEXITSTATUS(status) == EXIT_SUCCESS)
		{
			__D_DISPLAY("CGI SUCCESS, process response.");
		}
		else
			std::cout << "Cgi exited with status: "
				<< WEXITSTATUS(status) << std::endl;
	}
	else if (WIFSIGNALED(status))
		std::cerr << "Cgi exited with signal: "
			<< WTERMSIG(status) << std::endl;
	__D_DISPLAY("PARENT: finished to wait for cgi");
	return 0;
}

int		process_cgi(Response & response,
					const Request& request,
					const Location_config& location_block,
					const Server_config& server_block,
					const Client& client,
					const std::string & cgi_ext)
{
	pid_t	c_pid;
	int		ret;
	CgiEnv	env(request, location_block, server_block, client, cgi_ext);
	(void)response;

	__D_DISPLAY(env);

	//créer des pipes
	c_pid = fork();
	if (c_pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (c_pid == 0)//child
	{
		//dup2() les pipes avec les stdin/stdout
		//close tout les fd useless. (tout sauf les pipes)
		ret = execve((env.getArgv())[0], env.getArgv(), env.getEnv());
		if (ret == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
		__D_DISPLAY("iNtO fOrK AfTer eXeCvE & ExIt, sHouLDnT bE prInTEd");
	}
	else
		wait_cgi_status(c_pid);
	return 0;
}
//int execve(const char *pathname, char *const argv[], char *const envp[]);

//pathname must be formated as: /absolutepath/name. Ex: "/bin/ls"
//argv[0] must hold [name | path/name]. Ex: ["ls" | "bin/ls"]
//argv[n] holds arguments to the bin arguments. Ex: "-la"
//argv[end] must be NULL.
