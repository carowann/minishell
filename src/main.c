/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/12 19:38:40 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char *argv[])
{

	char			*input;
	t_token_list	*tokens;

	(void)argv;

	if (argc != 1)
	{
		ft_putstr_fd("Usage: ./minishell\n", 2);
		return (1);
	}
	printf("Minishell test\n"); //test
	while (1)
	{
		input = readline("minishell> ");
		if (!input) //Ctrl-D
			break;
		tokens = tokenize(input);
		//parse input
		//execute commands
		printf("Input: %s\n", input); //test
		free(input);
	}
	return (0);
}
