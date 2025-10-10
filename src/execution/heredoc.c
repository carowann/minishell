/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 16:42:45 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/10 17:29:12 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*read_heredoc_line(void)
{
	char	*line;

	if (g_signal_received == SIGINT)
		return (NULL);
	line = readline("> ");
	if (g_signal_received == SIGINT)
	{
		if (line)
			free(line);
		return (NULL);
	}
	return (line);
}

static void	process_and_write_line(char *line, int fd,
	t_shell_state *shell, int expand)
{
	if (expand && ft_strchr(line, '$'))
		line = expand_in_heredoc(line, shell);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

int	heredoc_read(int *pipefd, const char *del,
		t_shell_state *shell, int expand)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_line();
		if (!line && g_signal_received == SIGINT)
			return (-1);
		if (!line)
		{
			ft_printfd(2, "warning: here-document delimited by end-of-file ");
			ft_printfd(2, "(wanted `%s')\n", del);
			break ;
		}
		if (ft_strcmp(line, del) == 0)
		{
			free(line);
			break ;
		}
		process_and_write_line(line, pipefd[1], shell, expand);
	}
	close(pipefd[1]);
	return (0);
}

char	*expand_in_heredoc(char *line, t_shell_state *shell)
{
	char	*expanded_line;

	expanded_line = process_string_expansion(shell, line);
	free(line);
	return (expanded_line);
}

int	heredoc_sub(t_cmd *cmd, int *fd, t_shell_state *shell)
{
	signal(SIGINT, heredoc_exit_handler);
	signal(SIGQUIT, SIG_DFL);
	close(fd[0]);
	if (cmd->heredoc_count > 1)
		heredoc_read_placebo(cmd->heredoc_delimiters);
	if (heredoc_read(fd,
			cmd->heredoc_delimiter, shell, cmd->heredoc_expand) == -1)
	{
		close(fd[1]);
		pipe_free_all(cmd, shell);
		exit(130);
	}
	close(fd[1]);
	pipe_free_all(cmd, shell);
	return (0);
}
