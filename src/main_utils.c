/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:01:39 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/01 14:58:01 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Reads, returns line from terminal (interactive mode or not) and adds it to history
 * @return: input line
 */
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

/*
 * Checks if input is all spaces
 * @return: 0 no, 1 yes, ut's all spaces
 */
int	is_all_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!ft_isspace(input[i]))
			return (0);
		i++;
	}
	return (1);
}
