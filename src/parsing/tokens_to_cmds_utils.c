/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:57:50 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/23 12:37:55 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
 * Main loop that processes all tokens and builds commands
 * @param tokens: token list to process
 * @param cmd_list: command list to populate
 * @return: 0 success, -1 error
 */
int	process_token_loop(t_token_list *tokens, t_cmd_list *cmd_list)
{
	t_token	*curr_token;
	t_cmd	*curr_cmd;

	curr_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!curr_cmd)
		return (-1);
	curr_token = tokens->head;
	while (curr_token)
	{
		if (process_curr_token(&curr_token, &curr_cmd, cmd_list) == -1)
		{
			free_cmd(curr_cmd);
			return (-1);
		}
		curr_token = curr_token->next;
	}
	return (finalize_last_command(curr_cmd, cmd_list));
}

/*
 * Handles the last command after token processing
 * @param curr_cmd: current command to finalize
 * @param cmd_list: command list to update
 * @return: 0 success, -1 error
 */
int	finalize_last_command(t_cmd *curr_cmd, t_cmd_list *cmd_list)
{
	if (curr_cmd->arg_count > 0)
	{
		if (add_command_to_list(curr_cmd, cmd_list) == -1)
		{
			free_cmd(curr_cmd);
			return (-1);
		}
	}
	else
		free_cmd(curr_cmd);
	return (0);
}
