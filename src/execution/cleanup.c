/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:11:35 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/06 18:55:55 by lzorzit          ###   ########.fr       */
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