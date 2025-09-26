/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:42:45 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/26 16:19:34 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_in_heredoc(char *line, t_shell_state *shell);

// int handle_heredoc(t_cmd *cmd, t_shell_state **shell)
// {
// 	int   pipefd[2];
// 	pid_t pid;
// 	int   status;

// 	if (pipe_error(pipefd) == 1)
// 		return (1);
// 	pid = fork();
// 	if (pid == -1)
// 		return (fork_error(pipefd, NULL, NULL, NULL));
// 	if (pid == 0) //child uno, legge da stdin e scrive su pipefd[1]
// 		exit(doc_child_write(cmd, pipefd, shell));
// 	else
// 	{
// 		close(pipefd[1]);
// 		waitpid(pid, &status, 0);
// 		pid = fork();
// 		if (pid == -1)
// 			return (fork_error(pipefd, NULL, NULL, NULL));
// 		if (pid == 0) //child due, legge da pipefd[0] e duplica su stdin
// 			exit(doc_child_read(cmd, pipefd, shell));
// 		close(pipefd[0]);
// 		waitpid(pid, &status, 0);
// 		return (set_last_exit_status(*shell, status));
// 	}
// }

int heredoc_read(int *pipefd, const char *delimiter, t_shell_state *shell)
{
	char *line;

	while (1)
	{
		ft_printfd(1, "> ");
		line = read_line();
		if (!line || strcmp(line, delimiter) == 0)
			break;
		if (ft_strchr(line, '$'))
			line = expand_in_heredoc(line, shell);
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	if (line)
		free(line);
	else if (!line) // && g_signal == NOT_RECEIVED
		ft_printfd(2, "warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
	close(pipefd[1]);
	return (0);
}

char *expand_in_heredoc(char *line, t_shell_state *shell)
{
	char *expanded_line;

	expanded_line = process_string_expansion(shell, line);
	free(line);
	return (expanded_line);
}

// scommentare heredoc_read_placebo quando **delimiters è implementato
int heredoc_sub(t_cmd *cmd, int *fd, t_shell_state *shell)
{
	close(fd[0]);
	if (cmd->heredoc_delimiters[1])
		heredoc_read_placebo(fd, cmd->heredoc_delimiters, shell);
	else
		heredoc_read(fd, cmd->heredoc_delimiter, shell);
	close(fd[1]);
	if (!cmd->next)
	{
		pipe_free_all(cmd, shell);
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->heredoc_delimiter = NULL;
		cmd->input_file = NULL;
		free_cmd(cmd);
	}
	else
		pipe_free_all(cmd, shell);
	return (0);
}

int doc_child_write(t_cmd *cmd, int *fd, t_shell_state **shell)
{
	close(fd[0]);
	heredoc_read(fd, cmd->heredoc_delimiter, *shell);
	close(fd[1]);
	pipe_free_all((*shell)->current_cmd_list->head, *shell);
	return(0);
}

int doc_child_read(t_cmd *cmd, int *fd, t_shell_state **shell)
{
	int	status_code;
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (is_valid_cmd(cmd->args[0]))
		handle_builtin(cmd, shell);
	else
		handle_external_command(cmd, shell);
	status_code = (*shell)->last_exit_status;
	pipe_free_all((*shell)->current_cmd_list->head, *shell);
	return(status_code);
}
