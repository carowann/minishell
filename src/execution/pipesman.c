/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesman.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:00:38 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/05 15:09:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

// Manages the piping between two commands
int	pipeman(t_cmd *cmd_left, t_cmd	*cmd_right, t_shell_state *shell)
{
	pid_t	left_pid, right_pid;
	int		pipefd[2];
	int		status;
	
	fflush(NULL);
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (-1);
	}
	left_pid = fork();
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
		exec_pipeline_and_exit(cmd_left, shell, pipefd, 1);
	}
	right_pid = fork();
	if (right_pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		//kill(left_pid, SIGTERM);  // Termina il primo child
		waitpid(left_pid, NULL, 0);
		return (-1);
	}
	if (right_pid == 0)
		exec_pipeline_and_exit(cmd_right, shell, pipefd, 0);
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
		
	return (shell->last_exit_status);
}

// Executes command in pipeline and exits child process
void exec_pipeline_and_exit(t_cmd *cmd, t_shell_state *shell, int *fd, int flag)
{
	int result;
	
	if(flag == 0)  // Right command
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	else  // Left command
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);	
	}
	
	result = execute_cmd(cmd, &shell);
	if (shell->current_cmd_list)
		free_command_list(shell->current_cmd_list);
	free_env(shell->env_list);
	free(shell);
	exit(result);
}


// int pipe_free_all(t_cmd *cmd_left, t_cmd *cmd_right, t_shell_state *shell)
// {
// 	if (shell->current_cmd_list)
// 	{
// 		free(shell->current_cmd_list);
// 		shell->current_cmd_list = NULL;
// 	}
// 	free_env(shell->env_list);
// 	free(shell);
// 	if (!cmd_left || !cmd_right)
// 		return (-1);
// 	if (cmd_left)
// 		free_cmd(cmd_left);
// 	if (cmd_right)
// 		free_command_all(cmd_right);
// 	return (0);
// }

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
	return ;
}