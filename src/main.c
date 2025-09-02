/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/02 12:46:53 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
	char			*input;
	t_cmd_list		*commands;
	t_shell_state	*shell;
	(void)argv;

	if (argc != 1)
	{
		ft_putstr_fd(RED"Usage:	./minishell\n"RESET, STDERR_FILENO);
		return (1);
	}
	shell = ft_calloc(1, sizeof(t_shell_state));
	if (!shell)
		return (1);
	if (init_shell_state(shell, envp) == -1)
		return (1);
	while (1)
	{
		input = read_input_line();
		if (!input)
			break;
		if (ft_strlen(input) == 0 || is_all_spaces(input))
		{
			free(input);
			continue;
		}
		if (parse_input(input, &commands, &shell) == -1)
		{
			ft_putstr_fd(RED"Error while parsing\n"RESET, STDERR_FILENO);
			if (commands)
				free_command_list(commands);
			free (input);
			continue;
		}
		printf("Parsing successful!\n"); //debug
		execute_cmd(commands->head, &shell); //poi passer; shell state, cosi puo aggiornare exit status
		free_command_list(commands);
		free(input);
	}
	free_env(shell->env_list);
	free(shell);
	return (0);
}
