/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:49:25 by lzorzit           #+#    #+#             */
/*   Updated: 2025/08/22 17:04:18 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include "templib.h"

int env(int fd, t_env *env, int print_all)
{
	t_env *copy;

	copy = env;	
	while (copy)
	{
		if (copy->value && ft_strchr(copy->value, '='))
		{
			ft_printfd(fd, "%s\n", copy->value);
		}
		else if (copy->value && print_all == 1)
			ft_printfd(fd, "%s\n", copy->value);
			
		if (copy->next == NULL)
			break ;
		else
			copy = copy->next;
	}
	return (1);
}