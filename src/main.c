/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/13 18:07:23 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main()
{
	char			*input;
	t_token_list	*tokens;

	while (1)
	{
		input = readline("minishell> ");
		if (!input) //Ctrl-D
			break;
		if (tokenize(input, &tokens) == -1)
		{
			ft_putstr_fd("Error while parsing\n", 2);
			free (input);
			continue;
		}
			//execute commands
		printf("Input: %s\n", input); //test
		printf("Tokens creati: %d\n", tokens->count); //test
		free_token_list(tokens);
		free(input);
	}
	return (0);
}
