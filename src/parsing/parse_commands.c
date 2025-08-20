/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 13:57:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/20 11:22:19 by cwannhed         ###   ########.fr       */
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
		if (curr_token->type == WORD || curr_token->type == VARIABLE || curr_token->type == QUOTED_STRING)
		{
			if (add_arg_to_command(curr_token->content, curr_cmd) == -1)
			{
				free (curr_cmd);
				return (-1);
			}
		}
		else if (curr_token->type == PIPE)
		{
			if (add_command_to_list(curr_cmd, cmd_list) == -1)
			{
				free (curr_cmd);
				return (-1);
			}
			curr_cmd = ft_calloc(1, sizeof(t_cmd));
			if (!curr_cmd)
				return (-1);
		}
		curr_token = curr_token->next;
	}
	if (add_command_to_list(curr_cmd, cmd_list) == -1)
	{
		free (curr_cmd);
		return (-1);
	}
	return (0);
}

/*
 * Adds argument to array of arguments of command
 * @param arg: argument to append to array
 * @param cmd: command to update with new argument
 * @return: 0 success, -1 error
 */
int	add_arg_to_command(char *arg, t_cmd *cmd)
{
	char	**new_args;
	int		i;
	
	if (!arg || !cmd)
		return (-1);
	new_args = ft_calloc(cmd->arg_count + 2, sizeof(char *));
	if (!new_args)
		return (-1);
	i = 0;
	while (i < cmd->arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[cmd->arg_count] = ft_strdup(arg);
	if (!new_args[cmd->arg_count])
	{
		free(new_args);
		return (-1);
	}
	new_args[cmd->arg_count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_count++;
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
