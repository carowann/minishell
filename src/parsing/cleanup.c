/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:45:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/25 11:29:01 by cwannhed         ###   ########.fr       */
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
	while (curr_token)
	{
		temp_token = curr_token->next;
		free_token(curr_token);
		curr_token = temp_token;
	}
	free(token_list);
}

/*
 * Frees entire command list and all single commands
 * @param cmd_list: command list to free (can be NULL)
 */
void	free_command_list(t_cmd_list *cmd_list)
{
	t_cmd	*curr_cmd;
	t_cmd	*temp_cmd;

	if (!cmd_list)
		return ;
	if (cmd_list->head)
	{
		curr_cmd = cmd_list->head;
		while (curr_cmd)
		{
			temp_cmd = curr_cmd->next;
			free_cmd(curr_cmd);
			curr_cmd = temp_cmd;
		}
	}
	free(cmd_list);
}

/*
 * Frees an array of strings and the array itself
 * @param array: array of strings to free (can be NULL)
 * @param count: number of elements in the array
 */
void	free_string_array(char **array, int count)
{
	int i;

	if (!array)
		return;
	i = 0;
	while (i < count)
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
}

/*
 * Frees redirect arrays in a command
 * @param cmd: command containing redirect arrays to free
 */
void free_redirect_arrays(t_cmd *cmd)
{
	if (!cmd)
		return;

	if (cmd->input_files)
	{
		free_string_array(cmd->input_files, cmd->input_count);
		cmd->input_files = NULL;
		cmd->input_count = 0;
	}
	if (cmd->output_files)
	{
		free_string_array(cmd->output_files, cmd->output_count);
		cmd->output_files = NULL;
	}
	if (cmd->output_modes)
	{
		free(cmd->output_modes);
		cmd->output_modes = NULL;
		cmd->output_count = 0;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;

	if (cmd->args)
		free_string_array(cmd->args, cmd->arg_count);
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	if (cmd->output_file)
		free(cmd->output_file);
	free_redirect_arrays(cmd);
	free(cmd);
}

void	free_heredoc_delimiters(char **delimiters, int count)
{
	int	i;

	if (!delimiters)
		return ;
	i = 0;
	while (i < count)
	{
		if (delimiters[i])
			free(delimiters[i]);
		i++;
	}
	free(delimiters);
}
