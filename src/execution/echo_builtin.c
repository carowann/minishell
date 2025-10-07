/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:02:22 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/27 18:58:51 by lzorzit          ###   ########.fr       */
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
	while (ft_strncmp(args[n_var], "-n", 2) == 0)
		n_var ++;
	if (echo_exec(args, n_var, fd) == 0)
		return (1);
	return (SUCCESS);
}
