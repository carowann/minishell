/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:38:43 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/10 18:55:05 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_read_placebo(char **delimiter)
{
	char	*line;
	int		i;

	i = 0;
	while (delimiter[i + 1])
	{
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				ft_printfd(2, "\nwarning: here-document delimited by end-of-file (wanted `%s')\n", delimiter[i]);
				break ;
			}
			if (strcmp(line, delimiter[i]) == 0)
			{
				free(line);
				break ;
			}
			free(line);
		}
		if (!line)
			return (0); // Esci se Ctrl+D
		i++;
	}
	return (0);
}
