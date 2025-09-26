/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:38:43 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/26 16:33:05 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int heredoc_read_placebo(char **delimiter)
{
	char *line;
	int i;

	i = 0;
	while (delimiter[i+1])
	{
		while (1)
		{
			ft_printfd(1, "> ");
			line = read_line();
			if (!line || strcmp(line, delimiter[i]) == 0)
				break;
		}
		i ++;
	}
	if (line)
		free(line);
	else if (!line) // && g_signal == NOT_RECEIVED
		ft_printfd(2, "warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
	return (0);
}

