/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesman.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:00:38 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/26 17:48:22 by lzorzit          ###   ########.fr       */
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

	fflush(NULL);
	set_up_heredoc(shell->current_cmd_list->head, shell);
	setup_pipeline_signals();
	if (pipe_error(pipefd) == 1)
	{
		setup_interactive_signals();
		return (-1);
	}
	left_pid = fork();
	if (left_pid == -1)
	{
		setup_interactive_signals();
		return (fork_error(pipefd, NULL, NULL, 0));
	}
	if (left_pid == 0)
		exit(exec_pipeline_left(cmd_left, shell, pipefd));
	right_pid = fork();
	if (right_pid == -1)
	{
		setup_interactive_signals();
		return (fork_error(pipefd, &left_pid, NULL, 0));
	}
	if (right_pid == 0)
		exit(exec_pipeline_right(cmd_right, shell, pipefd));
	fork_close(pipefd, &left_pid, &right_pid, &status);
	setup_interactive_signals();
	set_last_exit_status(shell, status);
	return (shell->last_exit_status);
}

int	set_up_heredoc(t_cmd *cmd, t_shell_state *shell)
{
	int fd[2];
	char *line;
	pid_t pid;

	while (cmd)
	{
		if (cmd->is_heredoc == 1)
		{
			if (pipe_error(fd) == 1)
				return (1);
			pid = fork(); //creo un processo figlio che scrive nel pipe
			if (pid == 0)
				exit (heredoc_sub(cmd, fd, shell));
			waitpid(pid, NULL, 0);
			close(fd[1]);
			line = get_all_line(fd[0]);
			close(fd[0]);
			if (!line)
				cmd->heredoc_delimiter = NULL;
			else
				cmd->heredoc_delimiter = ft_strdup(line);
			cmd->is_heredoc = 2; //uso 2 per dire che e' stato gia' letto
			free(line);
		}
		cmd = cmd->next;
	}
	return (0);
}

int exec_pipeline_left(t_cmd *cmd, t_shell_state *shell, int *fd)
{
	int result;

	setup_default_signals();
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

int exec_pipeline_right(t_cmd *cmd, t_shell_state *shell, int *fd)
{
	int result;

	setup_default_signals();
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	result = execute_cmd(cmd, &shell);
	pipe_free_all(shell->current_cmd_list->head, shell);
	return (result);
}

int pipe_free_all(t_cmd *cmd_left, t_shell_state *shell)
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
