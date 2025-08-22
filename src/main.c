/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/22 18:13:31 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
	char		*input;
	t_cmd_list	*commands;
	t_env		*env;
	(void)argv;
	(void)envp;
	
	if (argc != 1)
	{
		ft_putstr_fd("Usage:	./minishell\n", 2);
		return (1);
	}
	//creazione envp list
	while (1)
	{
		input = readline(BOLD"minishell> "RESET);
		if (!input) //Ctrl-C
			break;
		if (parse_input(input, &commands, &env) == -1)
		{
			ft_putstr_fd("Error while parsing\n", 2);
			free (input);
			continue;
		}
		printf("Parsing successful!\n"); //debug
		//execute_commands(commands, envp);
		//free env list
		free_command_list(commands);
		free(input);
	}
	return (0);
}
