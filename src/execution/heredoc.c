/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:42:45 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/27 19:09:04 by lzorzit          ###   ########.fr       */
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
	char	*line;

	write(pipefd[1], "", 0); // Per evitare potenziali problemi di empty writes
	while (1)
	{
		ft_printfd(1, "> ");
		line = read_line();
		if (!line || ft_strcmp(line, delimiter) == 0) //TODO: era strcmp!!!! check forbidden functions
			break;
		if (ft_strchr(line, '$'))
			line = expand_in_heredoc(line, shell);
		write(pipefd[1], line, ft_strlen(line)); //TODO: era strlen normale!!!! check forbidden functions
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

int heredoc_sub(t_cmd *cmd, int *fd, t_shell_state *shell)
{
	    signal(SIGINT, heredoc_exit_handler);  // ← Cambia questa linea
    signal(SIGQUIT, SIG_DFL);              // ← Aggiungi questa linea
    
	close(fd[0]);
	if (cmd->heredoc_count > 1)
		heredoc_read_placebo(cmd->heredoc_delimiters);
	heredoc_read(fd, cmd->heredoc_delimiter, shell);
	close(fd[1]);
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
