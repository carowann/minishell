/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:49:25 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/01 17:34:50 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

int env(t_env *env, int print_all)
{
	t_env *copy;

	copy = env;	
	while (copy)
	{

		if (copy->value && print_all == 1)
		{
			ft_printf("declare -x ");
			ft_printf("%s\n", copy->value);
		}
		else if (copy->value && ft_strchr(copy->value, '='))
			ft_printf("%s\n", copy->value);	
		if (copy->next == NULL)
			break ;
		else
			copy = copy->next;
	}
	return (1);
}
