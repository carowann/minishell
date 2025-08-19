/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:57:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/18 17:27:19 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	tokens_to_commands(t_token_list *tokens, t_cmd_list *commands)
{
	t_token	*curr_token;
	t_cmd	*curr_cmd;

	if (!tokens || !commands)
		return (-1);
	curr_token = tokens->head;
	curr_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!curr_cmd)
		return (-1);
	while (curr_token)
	{
		if (curr_token->type == WORD || curr_token->type == VARIABLE)
		{
			//TODO: add_arg_to_command(curr_token, curr_cmd)
		}
		else if (curr_token->type == PIPE)
		{
			//TODO: add_command_to_list
			curr_cmd = ft_calloc(1, sizeof(t_cmd));
			if (!curr_cmd)
				return (-1);
		}
		curr_token = curr_token->next;
	}
	//add_command_to_list
	return (0);
}

int	add_arg_to_command(t_token *token, t_cmd *cmd)
{
	if (!token || !cmd)
		return (-1);
	return (0);
}
