/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:01:39 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/04 12:27:42 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Main shell loop that reads input, parses it, and executes commands
 * Continues until should_exit flag in shell state is set
 * @param shell: pointer to shell state struct
 * @return: void
 */
void shell_loop(t_shell_state **shell)
{
	t_cmd_list	*commands;
	char		*input;

	while (!(*shell)->should_exit)
	{
		commands = NULL;
		input = read_input_line();
		if (!input)
			break;
		if (ft_strlen(input) == 0 || is_all_spaces(input))
		{
			free(input);
			continue;
		}
		if (parse_input(input, &commands, shell) == -1)
		{
			(*shell)->last_exit_status = 258;
			ft_putstr_fd(RED "Error while parsing\n" RESET, STDERR_FILENO);
			if (commands)
				free_command_list(commands);
			free(input);
			continue;
		}
		free(input);
		(*shell)->current_cmd_list = commands;
		execute_cmd(commands->head, shell);
		free_command_list(commands);
		(*shell)->current_cmd_list = NULL;
	}
	return;
}

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

/*
 * Initialises shell state struct 
 * @param shell: struct to initialise
 * @param envp
 * @return: 0 no, 1 yes, ut's all spaces
 */
int init_shell_state(t_shell_state *shell, char **envp)
{
	shell->env_list = env_to_list(envp);
	if (!shell->env_list)
		return (-1);
	shell->last_exit_status = 0;
	shell->should_exit = 0;
	shell->exit_code = 0;
	shell->current_cmd_list = NULL;
	return (0);
}
