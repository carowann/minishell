/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesman.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:00:38 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/05 17:49:45 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Manages the piping between two commands
int	pipeman(t_cmd *cmd_left, t_cmd	*cmd_right, t_shell_state *shell)
{
	pid_t	left_pid;
	pid_t	right_pid;
	int		pipefd[2];
	int		status;
	int		cmd_result;
	
	fflush(NULL);
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	left_pid = fork();
	printf("Created left process with PID %d\n", left_pid); // Debug
	if (left_pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (left_pid == 0)
	{
		cmd_left->next = NULL;			
		cmd_result = exec_pipeline(cmd_left, shell, pipefd, 1);
		pipe_free_all(cmd_left, cmd_right, shell);
		exit(cmd_result);
	}
	right_pid = fork();
	printf("Created right process with PID %d\n", right_pid); // Debug
	if (right_pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(left_pid, NULL, 0);
		return (-1);
	}
	if (right_pid == 0)
	{
		cmd_result = exec_pipeline(cmd_right, shell, pipefd, 0);
		pipe_free_all(cmd_left, cmd_right, shell);
		exit(cmd_result);
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
	ft_printfd(STDERR_FILENO, "Left PID: %d, Right PID: %d, Exit Status: %d\n", left_pid, right_pid, shell->last_exit_status); // Debug
	return (shell->last_exit_status);
}

// Executes a command in a pipeline, flag indicates if it's left (1) or right (0)
int	exec_pipeline(t_cmd *cmd, t_shell_state *shell, int *fd, int flag)
{
	int result;
	
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
	result = execute_cmd(cmd, &shell);
	return (result);
}

int pipe_free_all(t_cmd *cmd_left, t_cmd *cmd_right, t_shell_state *shell)
{
	ft_printfd(2, "Freeing all resources in pipe_free_all in process %d\n", getpid()); // Debug
	ft_printfd(2, "Current command list in shell state: %s\n", shell->current_cmd_list ? "not NULL" : "NULL"); // Debug
	if (shell->current_cmd_list)
	{
		free(shell->current_cmd_list);
		shell->current_cmd_list = NULL;
	}
	free_env(shell->env_list);
	free(shell);
	if (cmd_left)
		free_cmd(cmd_left);
	if (cmd_right)
		free_command_all(cmd_right);
	return (0);
}

void	free_command_all(t_cmd *cmd)
{
	t_cmd	*temp_cmd;

	if (!cmd || (!cmd->next))
		return ;
	while (cmd && cmd->next)
	{	
		temp_cmd = cmd->next;
		free_cmd(cmd);
		cmd = temp_cmd;
	}
	free_cmd(cmd);
	return ;
}
