
#include "../../includes/minishell.h"
#include <sys/wait.h>

// Manages the piping between two commands
int	pipeman(t_cmd *cmd_left, t_cmd	*cmd_right, t_shell_state *shell)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pipefd[2];
	int		status;
	
	fflush(NULL);
	pipe(pipefd);
	left_pid = fork();
	if (left_pid == 0)
	{
		cmd_left->next = NULL;
		exec_pipeline(cmd_left, shell, pipefd, 1);
		exit(pipe_free_all(cmd_left, cmd_right, shell));
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		exec_pipeline(cmd_right, shell, pipefd, 0);
		exit(pipe_free_all(cmd_left, cmd_right, shell));
	}
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, &status, 0);
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
	else 
		shell->last_exit_status = 1;
	cmd_left->next = NULL;
	return (shell->last_exit_status);
}

// Executes a command in a pipeline, flag indicates if it's left (1) or right (0)
int	exec_pipeline(t_cmd *cmd, t_shell_state *shell, int *fd, int flag)
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

int	pipe_free_all(t_cmd *cmd_left, t_cmd *cmd_right, t_shell_state *shell)
{	
	free_env(shell->env_list);
	free(shell);
	if(!cmd_left || !cmd_right)
		return (-1);
	if (cmd_left)
		free_cmd(cmd_left);
	if (cmd_right)
		free_command_all(cmd_right);
	return (0);
}

void	free_command_all(t_cmd *cmd)
{
	t_cmd	*temp_cmd;
	if (!cmd)
		return ;
	while (cmd && cmd->next)
	{	
		temp_cmd = cmd->next;
		free_cmd(cmd);
		cmd = temp_cmd;
	}
	free_cmd(cmd);
	return	;
}
