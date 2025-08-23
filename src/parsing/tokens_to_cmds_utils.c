/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:57:50 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/23 13:26:45 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_argument_token(t_token *token)
{
	if ((token->type == WORD 
		|| token->type == VARIABLE 
		|| token->type == QUOTED_STRING)
		&& (token->content && strlen(token->content) > 0))
		return (1);
	return (0);
}
