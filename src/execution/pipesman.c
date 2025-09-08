/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesman.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:00:38 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/06 19:33:29 by lzorzit          ###   ########.fr       */
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
	if (left_pid == -1)
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (left_pid == 0)
	{	
		ft_printfd(1, "Left PID: %d,%s\n", getpid(), fgets(NULL, 0, stdin));
		cmd_result = exec_pipeline_left(cmd_left, shell, pipefd);
		pipe_free_all(cmd_left, shell);
		exit(cmd_result);
	}
	
	right_pid = fork();
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
		cmd_result = exec_pipeline_right(cmd_right, shell, pipefd);
		pipe_free_all(cmd_left, shell);
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
	return (shell->last_exit_status);
}
int	pipe_heredoc_changes(t_cmd *cmd)
{
	int fd[2];
	pid_t pid;

	if (pipe(fd) == -1)
	{
		perror("pipe in heredoc failed");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork in heredoc failed");
		close(fd[0]);
		close(fd[1]);
		return (1);
	}
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_read(fd, cmd->heredoc_delimiter);
		close(fd[1]);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	return (0);
}
int exec_pipeline_left(t_cmd *cmd, t_shell_state *shell, int *fd)
{
	if(cmd->is_heredoc == 1)
		pipe_heredoc_changes(cmd);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (is_valid_cmd(cmd->args[0]))
		return (handle_builtin(cmd, &shell));
	return (handle_external_command(cmd, &shell));	
}
int exec_pipeline_right(t_cmd *cmd, t_shell_state *shell, int *fd)
{
	int result;

	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	result = execute_cmd(cmd, &shell);
	return (result);
}

int pipe_free_all(t_cmd *cmd_left, t_shell_state *shell)
{
	if (cmd_left)
		free_command_all(shell->current_cmd_list->head);
	if (shell->current_cmd_list)
	{
		free(shell->current_cmd_list);
		shell->current_cmd_list = NULL;
	}
	free_env(shell->env_list);
	free(shell);
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

// Executes a command in a pipeline, flag indicates if it's left (1) or right (0)
// int	exec_pipeline(t_cmd *cmd, t_shell_state *shell, int *fd, int flag)
// {
// 	int result;
// 	if(flag == 0)
// 	{
// 		close(fd[1]);
// 		if(cmd->is_heredoc == 1)
// 			close(fd[0]);
// 		dup2(fd[0], STDIN_FILENO);
// 		close(fd[0]);
// 	}
// 	else
// 	{
// 		close(fd[0]);
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[1]);	
// 	}
// 	if (flag == 1)
// 	{
// 		if (is_valid_cmd(cmd->args[0]))
// 			return (handle_builtin(cmd, &shell));
// 		return (handle_external_command(cmd, &shell));	
// 	}
// 	result = execute_cmd(cmd, &shell);
// 	return (result);
// }