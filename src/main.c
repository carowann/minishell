/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:09:04 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/05 12:47:11 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_shell_state *shell;
	int exit_code;
	(void)argv;

	if (argc != 1)
	{
		ft_putstr_fd(RED "Usage: ./minishell\n" RESET, STDERR_FILENO);
		return (1);
	}
	shell = ft_calloc(1, sizeof(t_shell_state));
	if (!shell)
		return (1);
	if (init_shell_state(shell, envp) == -1)
	{
		free(shell);
		return (1);
	}
	shell_loop(&shell);
	exit_code = shell->exit_code;
	free_env(shell->env_list);
	free(shell);
	return (exit_code);
}
