/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:01:39 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/13 18:19:30 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Handle parsing error and cleanup
 */
static int	handle_parse_error(char **input,
	t_cmd_list **cmds,
	t_shell_state **shell)
{
	(*shell)->last_exit_status = 258;
	free(*input);
	if (*cmds)
	{
		free_command_list(*cmds);
		(*shell)->current_cmd_list = NULL;
	}
	return (1);
}

/*
 * Execute command and cleanup
 */
static void	execute_and_cleanup(char **input,
	t_cmd_list **cmds,
	t_shell_state **shell)
{
	free(*input);
	(*shell)->current_cmd_list = *cmds;
	execute_cmd((*cmds)->head, shell);
	if (*cmds)
	{
		free_command_list(*cmds);
		(*shell)->current_cmd_list = NULL;
	}
}

/*
 * Process single input line
 * @return: 0 continue, 1 exit loop
 */
static int	process_input_line(t_shell_state **shell)
{
	t_cmd_list	*commands;
	char		*input;

	commands = NULL;
	input = read_input_line(shell);
	if (!input)
		return (0);
	if (parse_input(input, &commands, shell) == -1)
		return (handle_parse_error(&input, &commands, shell));
	execute_and_cleanup(&input, &commands, shell);
	return (0);
}

/*
 * Main shell loop that reads input, parses it, and executes commands
 * Continues until should_exit flag in shell state is set
 * @param shell: pointer to shell state struct
 * @return: void
 */
void	shell_loop(t_shell_state **shell)
{
	while (!(*shell)->should_exit)
	{
		if (process_input_line(shell))
			continue ;
	}
	return ;
}

/*
 * Reads, returns line from terminal (interactive mode or not)
 * and adds it to history
 * @return: input line or NULL
 */
char	*read_input_line(t_shell_state **shell)
{
	char	*input;

	input = readline(BOLD "minishell> " RESET);
	if (input && *input)
		add_history(input);
	if (g_signal_received == SIGINT)
	{
		(*shell)->last_exit_status = 130;
		g_signal_received = 0;
	}
	if (!input)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		(*shell)->should_exit = 1;
		(*shell)->exit_code = (*shell)->last_exit_status;
		return (NULL);
	}
	if (ft_strlen(input) == 0 || is_all_spaces(input))
	{
		free(input);
		return (NULL);
	}
	return (input);
}
