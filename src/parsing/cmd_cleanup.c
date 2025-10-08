/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:59:44 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/08 17:05:25 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Frees entire command list and all single commands
 * @param cmd_list: command list to free (can be NULL)
 */
void free_command_list(t_cmd_list *cmd_list)
{
	t_cmd *curr_cmd;
	t_cmd *temp_cmd;

	if (!cmd_list)
		return;
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
	cmd_list = NULL;
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

void free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return;
	if (cmd->args)
		free_string_array(cmd->args, cmd->arg_count);
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	if (cmd->heredoc_delimiters)
		free_heredoc_delimiters(cmd->heredoc_delimiters);
	if (cmd->input_file)
	{
		free(cmd->input_file);
		cmd->input_file = NULL;
	}
	if (cmd->output_file)
	{
		free(cmd->output_file);
		cmd->output_file = NULL;
	}
	free_redirect_arrays(cmd);
	free(cmd);
}

void free_heredoc_delimiters(char **delimiters)
{
	int i;

	if (!delimiters)
		return;
	i = 0;
	while (delimiters[i])
	{
		free(delimiters[i]);
		i++;
	}
	free(delimiters);
}
