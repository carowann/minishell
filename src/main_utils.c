/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:01:39 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/27 15:27:19 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*read_input_line(void)
{
	char	*input;
	size_t	len;

	if (isatty(STDIN_FILENO))
	{
		input = readline(BOLD"minishell> "RESET);
		if (input && *input)
			add_history(input);
	}
	else
	{
		input = get_next_line(STDIN_FILENO);
		if (!input)
			return (NULL);
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
	}
	return (input);
}
