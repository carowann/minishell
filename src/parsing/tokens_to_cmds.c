/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:57:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/26 17:37:33 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Parses token list into command list ready for execution
 * @param tokens: token list to convert in command list
 * @param commands: output command list
 * @return: 0 success, -1 error
 */
int	tokens_to_commands(t_token_list *tokens, t_cmd_list *cmd_list)
{
	t_token	*curr_token;
	t_cmd	*curr_cmd;

	if (!tokens || !cmd_list)
		return (-1);
	curr_token = tokens->head;
	curr_cmd = ft_calloc(1, sizeof(t_cmd));
	if (!curr_cmd)
		return (-1);
	while (curr_token)
	{
		if (process_curr_token(&curr_token, &curr_cmd, cmd_list) == -1)
			return (cleanup_and_return_error(curr_cmd));
		curr_token = curr_token->next;
	}
	if (add_command_to_list(curr_cmd, cmd_list) == -1)
		cleanup_and_return_error(curr_cmd);
	return (0);
}

/*
 * Analyses current tokens's type, creates relative command and adds it to the list
 * @param curr_token: token to analyze
 * @param curr_cmd: command to populate w/ token info
 * @param cmd_list: list of all commands
 * @return: 0 success, -1 error
 */
int	process_curr_token(t_token **curr_token, t_cmd **curr_cmd, t_cmd_list *cmd_list)
{
	if (is_argument_token(*curr_token) 
		&& (*curr_token)->content
		&& strlen((*curr_token)->content) > 0)
		return (handle_argument_token(*curr_token, *curr_cmd));
	else if ((*curr_token)->type == PIPE)
		return (handle_pipe_token(curr_cmd, cmd_list));
	else if (is_redirect_token(*curr_token))
		return (handle_redirect_token(curr_token, *curr_cmd));
	return (0);
}

/*
 * Adds command to list of commands
 * @param new_cmd: new command to add to list
 * @param cmd_list: output command list
 * @return: 0 success, -1 error
 */
int	add_command_to_list(t_cmd *new_cmd, t_cmd_list *cmd_list)
{
	t_cmd	*curr_cmd;
	
	if (!new_cmd || !cmd_list)
		return (-1);
	if (!cmd_list->head)
	{
		cmd_list->head = new_cmd;
		cmd_list->count++;
		return (0);
	}
	curr_cmd = cmd_list->head;
	while (curr_cmd->next)
		curr_cmd = curr_cmd->next;
	curr_cmd->next = new_cmd;
	cmd_list->count++;
	return (0);
}
