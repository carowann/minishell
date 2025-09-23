/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:27:41 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/23 18:16:26 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_exit(t_cmd *cmd, t_shell_state *shell)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!cmd->args[1])
		exit_code = shell->last_exit_status;
	else if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else
		exit_code = validate_exit_arg(cmd->args[1]);
	shell->should_exit = 1;
	shell->exit_code = exit_code;
	return (exit_code);
}

int	is_valid_number(char *arg)
{
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg)
	{
		if (!ft_isdigit(*arg))
			return (0);
		arg++;
	}
	return (1);
}

int	validate_exit_arg(char *arg)
{
	int	exit_code;

	if (!is_valid_number(arg))
	{
		ft_printfd(STDERR_FILENO, "minishell: exit: %s: invalid arg\n", arg);
		exit_code = 2;
	}
	else
	{
		exit_code = ft_atoi(arg);
		exit_code = exit_code % 256;
		if (exit_code < 0)
			exit_code = 256 + exit_code;
	}
	return (exit_code);
}
