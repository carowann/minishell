/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:27:41 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/16 18:31:31 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_exit(t_cmd *cmd, t_shell_state *shell, int output_fd)
{
	int	exit_code;

	if (output_fd != STDOUT_FILENO && output_fd >= 0)
		close(output_fd);
	if (!shell->is_child)
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
	if (shell->is_child)
	{
		pipe_free_all(shell->current_cmd_list->head, shell);
		exit(exit_code);
	}
	shell->should_exit = 1;
	shell->exit_code = exit_code;
	return (exit_code);
}

/*
 * Converts string to long long and detects overflow
 * @param str: string to convert
 * @param result: pointer to store result
 * @return: 1 if valid, 0 if overflow/invalid
 */
static int	safe_atoll(const char *str, long long *result)
{
	long long	num;
	int			sign;
	int			digit;

	num = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		digit = *str - '0';
		if (sign == 1 && (num > (LLONG_MAX - digit) / 10))
			return (0);
		if (sign == -1 && (num > (LLONG_MAX - digit) / 10))
			return (0);
		num = num * 10 + digit;
		str++;
	}
	*result = num * sign;
	return (1);
}

static int	is_valid_number(char *arg)
{
	if (*arg == '-' || *arg == '+')
		arg++;
	if (*arg == '\0')
		return (0);
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
	long long	ll_exit_code;
	int			exit_code;

	if (!is_valid_number(arg) || !safe_atoll(arg, &ll_exit_code))
	{
		ft_printfd(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", arg);
		return (2);
	}
	exit_code = (int)(ll_exit_code % 256);
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	return (exit_code);
}
