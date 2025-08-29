/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:57:50 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/26 17:36:00 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Checks if token type is valid to convert token in arguments for command
 * @param token: token to analyse
 * @return: 1 yes, 0 no
 */
int	is_argument_token(t_token *token)
{
	if ((token->type == WORD 
		|| token->type == VARIABLE 
		|| token->type == QUOTED_STRING))
		return (1);
	return (0);
}

/*
 * Checks if token type is valid for redirection
 * @param token: token to analyse
 * @return: 0 no, 1 yes
 */
int	is_redirect_token(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == REDIRECT_IN
		|| token->type == REDIRECT_OUT
		|| token->type == APPEND
		|| token->type == HEREDOC);
}

/*
 * Frees command struct and returns error
 * @param cmd: command to free
 * @return: -1 error
 */
int	cleanup_and_return_error(t_cmd *cmd)
{
	free_cmd(cmd);
	return (-1);
}
