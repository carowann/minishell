/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/21 17:58:45 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
	char		*input;
	t_cmd_list	*commands;
	(void)argc;
	(void)argv;
	(void)envp;
	
	while (1)
	{
		input = readline(BOLD"minishell> "RESET);
		if (!input) //Ctrl-C
			break;
		if (parse_input(input, &commands) == -1)
		{
			ft_putstr_fd("Error while parsing\n", 2);
			free (input);
			continue;
		}
		printf("Parsing successful!\n"); //debug
		//execute_commands(commands, envp);
		free_command_list(commands);
		free(input);
	}
	return (0);
}
