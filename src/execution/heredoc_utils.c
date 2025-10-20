/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:38:43 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/20 12:44:55 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	heredoc_read_loop(char **delimiter, char **line, int i)
{
	while (1)
	{
		*line = readline("> ");
		if (!*line && g_signal_received == SIGINT)
			return (-1);
		if (!*line)
		{
			ft_printfd(2, "\nwarning: here-document delimited by end-of-file");
			ft_printfd(2, " (wanted `%s')\n", delimiter[i]);
			return (0);
		}
		if (strcmp(*line, delimiter[i]) == 0)
		{
			free(*line);
			break ;
		}
		free(*line);
	}
	return (1);
}

int	heredoc_read_placebo(char **delimiter)
{
	char	*line;
	int		heredoc_status;
	int		i;

	i = 0;
	while (delimiter[i + 1])
	{
		heredoc_status = heredoc_read_loop(delimiter, &line, i);
		if (heredoc_status == -1)
			return (-1);
		i++;
	}
	return (0);
}
