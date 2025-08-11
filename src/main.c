/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/11 13:16:55 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char *argv[])
{

	char	*input;

	(void)argc;
	(void)argv;

	printf("Minishell test\n");
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free (input);
			break;
		}
		printf("Input: %s\n", input);
	free(input);
	}
	return (0);
}