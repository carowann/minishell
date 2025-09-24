/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:49:25 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/23 18:25:01 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	env(t_env *env, int fd, int print_all)
{
	t_env	*copy;

	copy = env;
	while (copy)
	{
		if (copy->value && print_all == 1)
		{
			ft_printfd(fd, "declare -x ");
			ft_printfd(fd, "%s\n", copy->value);
		}
		else if (copy->value && ft_strchr(copy->value, '='))
			ft_printfd(fd, "%s\n", copy->value);
		if (copy->next == NULL)
			break ;
		else
			copy = copy->next;
	}
	return (SUCCESS);
}
