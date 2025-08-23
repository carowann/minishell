/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:57:50 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/23 19:38:01 by cwannhed         ###   ########.fr       */
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

int	is_redirect_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == REDIRECT_IN
		|| token->type == REDIRECT_OUT
		|| token->type == APPEND
		|| token->type == HEREDOC);
}

int	cleanup_and_return_error(t_cmd *cmd)
{
	free_cmd(cmd);
	return (-1);
}
