/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesman.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:00:38 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/20 19:31:25 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Manages the piping between two commands
int	pipeman(t_cmd *cmd_left, t_cmd	*cmd_right, t_shell_state *shell)
{
	int	pipefd[2];
	int	status;

	setup_signals(PIPELINE);
	if (set_up_heredoc(shell->current_cmd_list->head, shell) == -1)
	{
		setup_signals(INTERACTIVE);
		return (-1);
	}
	if (pipe_error(pipefd) == 1)
	{
		setup_signals(INTERACTIVE);
		return (-1);
	}
	if (cmd_right == NULL)
		return (printf("Error: cmd_right is NULL\n"), -1);
	if (fork_and_execute(cmd_left, &status, shell, pipefd) == -1)
		return (fork_close(pipefd, NULL, NULL, &status));
	setup_signals(INTERACTIVE);
	set_last_exit_status(shell, status);
	return (shell->last_exit_status);
}

int	set_up_heredoc(t_cmd *cmd, t_shell_state *shell)
{
	int		status;
	int		fd[2];
	pid_t	pid;

	while (cmd)
	{
		if (cmd->is_heredoc == 1)
		{
			if (pipe_error(fd) == 1)
				return (1);
			pid = fork();
			if (pid == 0)
				exit(heredoc_sub(cmd, fd, shell));
			waitpid(pid, &status, 0);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
				return (heredoc_status(fd, shell));
			heredoc_closing(cmd, fd);
		}
		cmd = cmd->next;
	}
	return (0);
}

int	exec_pipeline_left(t_cmd *cmd, t_shell_state *shell, int *fd)
{
	int	result;

	shell->is_child = 1;
	setup_signals(DFL);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	if (is_valid_cmd(cmd->args[0]))
		result = handle_builtin(cmd, &shell);
	else
		result = handle_external_command(cmd, &shell);
	pipe_free_all(cmd, shell);
	return (result);
}

int	exec_pipeline_right(t_cmd *cmd, t_shell_state *shell, int *fd)
{
	int	result;

	shell->is_child = 1;
	setup_signals(DFL);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	result = execute_cmd(cmd, &shell);
	pipe_free_all(shell->current_cmd_list->head, shell);
	return (result);
}

int	pipe_free_all(t_cmd *cmd_left, t_shell_state *shell)
{
	if (shell->current_cmd_list->head && cmd_left)
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
