/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ludovico <ludovico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:02:22 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/17 22:35:40 by ludovico         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//	execution of command after vars are passed by echo
int	echo_exec(char **str, int n_var, int fd)
{
	int	i;

	i = 1;
	while (i != n_var)
		i++;
	while (str[i])
	{
		if (str[i][0] != '\0')
			ft_printfd(fd, "%s", str[i]);
		i++;
		if (str[i] && str[i - 1][0] != '\0')
			ft_printfd(fd, " ");
	}
	if (n_var == 1)
		write(fd, "\n", 1);
	return (1);
}

//	command called when the Echo keyword is read
int	echo_builtin(char **args, int fd)
{
	int	n_var;

	n_var = 1;
	if (!args[1])
	{
		ft_printfd(fd, "%s", "\n");
		return (SUCCESS);
	}
	while (args[n_var] && echo_nskip(args[n_var]) == 0)
		n_var ++;
	if (echo_exec(args, n_var, fd) == 0)
		return (1);
	return (SUCCESS);
}

int echo_nskip(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (1);
	i++;
	if (str[i] == '\0')
		return (1);
	while (str[i] && str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (0);
	return (1);
}
