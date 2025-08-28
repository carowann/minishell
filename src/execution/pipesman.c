
#include "../../includes/minishell.h"
#include <sys/wait.h>

int	pipeman(t_cmd *cmd_left, t_cmd	*cmd_right, t_env *envar)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pipefd[2];
	
	fflush(NULL);
	pipe(pipefd);
	left_pid = fork();
	if (left_pid == 0)
	{
		cmd_left->next = NULL;
		exit(exec_pipeline(cmd_left, envar, pipefd[1], pipefd[0]));
	}
	right_pid = fork();
	if (right_pid == 0)
		exit(exec_pipeline(cmd_right, envar, pipefd[0], pipefd[1]));
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
	return (1);
}

int		exec_pipeline(t_cmd *cmd, t_env *envar, int fd, int fd_close)
{
	close(fd_close);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	execute_cmd(cmd, envar);
	return (0);
}