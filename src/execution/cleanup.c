/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:11:35 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/23 12:55:08 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_matrix(char **matrix)
{
	int	i;
	
	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

// Free the shell state and all its components, used in child processes
void  free_shell(t_shell_state *shell)
{
	if (!shell)
		return ;
	if (shell->env_list)
		free_env(shell->env_list);
	if (shell->current_cmd_list)
		free_command_all(shell->current_cmd_list->head);
	free(shell->current_cmd_list);
	free(shell);
}