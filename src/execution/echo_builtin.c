/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:02:22 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/06 12:52:40 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: cwannhed@c1r6p8:~$ echo -n -n hello

//	execution of command after vars are passed by echo
int	echo_exec(char **str, int n_var, int fd)
{
	int	i;

	i = 1;
	if (n_var == 1)
		i++;
	while (str[i])
	{
		if (str[i][0] != '\0')
			ft_printfd(fd, "%s", str[i]);
		i++;
		if (str[i] && str[i - 1][0] != '\0')
			ft_printfd(fd, " ");
	}
	if (n_var == 0)
		write(fd, "\n", 1);
	return (1);
}

//	command called when the Echo keyword is read
int	echo_builtin(char **args, int fd)
{
	int	n_var;

	if (!args[1])
	{
		ft_printfd(fd, "%s", "\n");
		return (SUCCESS);
	}
	if (ft_strncmp(args[1], "-n", 2) == 0)
		n_var = 1;
	else
		n_var = 0;
	if (echo_exec(args, n_var, fd) == 0)
		return (1);
	return (SUCCESS);
}
