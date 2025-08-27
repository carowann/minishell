/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/27 18:12:21 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
	char		*input;
	t_cmd_list	*commands;
	t_env		*env;
	(void)argv;

	if (argc != 1)
	{
		ft_putstr_fd(RED"Usage:	./minishell\n"RESET, 2);
		return (1);
	}
	env = env_to_list(envp);
	if (!env)
		return (1);
	while (1)
	{
		input = read_input_line();
		if (!input)
			break;
		if (ft_strlen(input) == 0)
			continue;
		if (parse_input(input, &commands, &env) == -1)
		{
			ft_putstr_fd(RED"Error while parsing\n"RESET, 2);
			free (input);
			continue;
		}
		printf(GREEN"Parsing successful!\n\n"RESET); //debug
		execute_cmd(commands->head, env);
		free_command_list(commands);
		free(input);
	}
	free_env(env);
	return (0);
}
