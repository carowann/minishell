/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:49:25 by lzorzit           #+#    #+#             */
/*   Updated: 2025/08/20 18:29:56 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include "templib.h"

int env(int fd, t_env *env)
{
	t_env *copy;

	copy = env;	
	while (copy)
	{
		if (copy->value)
		{
			ft_printfd(fd, "%s\n", copy->value);
		}
		if (copy->next == NULL)
			break ;
		else
			copy = copy->next;
	}
	return (1);
}