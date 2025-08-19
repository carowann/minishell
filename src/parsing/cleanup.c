/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:45:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/19 15:08:03 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Frees single token and its content
 * @param token: token to free (can be NULL)
 */
void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->content)
		free(token->content);
	free(token);
	token = NULL;
}

/*
 * Frees entire token list and all tokens
 * @param token_list: list to free (can be NULL)
 */
void	free_token_list(t_token_list *token_list)
{
	t_token	*curr_token;
	t_token	*temp_token;
	
	if (!token_list)
		return ;
	curr_token = token_list->head;
	while (curr_token && curr_token->next)
	{
		temp_token = curr_token->next;
		free_token(curr_token);
		curr_token = temp_token;
	}
	free_token(curr_token);
	free(token_list);
}

void	free_command_list(t_cmd_list *cmd_list)
{
	t_cmd	*curr_cmd;
	t_cmd	*temp_cmd;
	
	if (!cmd_list || !cmd_list->head)
		return ;
	curr_cmd = cmd_list->head;
	while (curr_cmd)
	{
		temp_cmd = curr_cmd->next;
		free_cmd(curr_cmd);
		curr_cmd = temp_cmd;
	}
	free(cmd_list);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}
