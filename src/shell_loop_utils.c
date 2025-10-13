/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 16:21:10 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/13 16:21:31 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
int	init_shell_state(t_shell_state *shell, char **envp)
{
	shell->env_list = env_to_list(envp);
	if (!shell->env_list)
		return (-1);
	shell->last_exit_status = 0;
	shell->should_exit = 0;
	shell->exit_code = 0;
	shell->is_child = 0;
	shell->current_cmd_list = NULL;
	return (0);
}
