
#include "../../includes/minishell.h"
#include <sys/wait.h>

// Manages the piping between two commands
int	pipeman(t_cmd *cmd_left, t_cmd	*cmd_right, t_shell_state *shell)
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
		exit(exec_pipeline(cmd_left, shell, pipefd, 1));
	}
	right_pid = fork();
	if (right_pid == 0)
		exit(exec_pipeline(cmd_right, shell, pipefd, 0));
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
	return (1);
}
// Executes a command in a pipeline, flag indicates if it's left (1) or right (0)
int		exec_pipeline(t_cmd *cmd, t_shell_state *shell, int *fd, int flag)
{
	if(flag == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);	
	}
	execute_cmd(cmd, &shell);
	return (0);
}
