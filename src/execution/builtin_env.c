/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ludovico <ludovico@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:49:25 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/14 17:10:57 by ludovico         ###   ########.fr       */
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
			print_env_export_format(fd, copy->value);
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

void print_env_export_format(int fd, char *env_var)
{
	int		i;
	char	*key;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	key = ft_substr(env_var, 0, i);
	if (!key)
		return ;
	if (env_var[i] == '=')
		ft_printfd(fd, "declare -x %s=\"%s\"\n", key, env_var + i + 1);
	else
		ft_printfd(fd, "declare -x %s\n", key);
	free(key);
}