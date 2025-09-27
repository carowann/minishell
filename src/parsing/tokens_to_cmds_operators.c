/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_operators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:43:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/27 19:12:12 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_input_file(t_cmd *cmd, char *filename)
{
	char	**new_array;
	int		i;

	new_array = malloc(sizeof(char *) * (cmd->input_count + 1));
	if (!new_array)
		return (-1);
	i = 0;
	while (i < cmd->input_count)
	{
		new_array[i] = cmd->input_files[i];
		i++;
	}
	new_array[cmd->input_count] = ft_strdup(filename);
	if (!new_array[cmd->input_count])
	{
		free(new_array);
		return (-1);
	}
	if (cmd->input_files)
		free(cmd->input_files);
	cmd->input_files = new_array;
	cmd->input_count++;
	return (0);
}

/*
 * Sets filename in input in cmd struct
 * @param cmd: command to modify
 * @param filename: filename to insert
 * @param curr_token: token to skip
 * @return: 0 success, -1 error
 */
int	set_input_redirect(t_cmd *cmd, char *filename, t_token **curr_token)
{
	if (add_input_file(cmd, filename) == -1)
		return (-1);
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(filename);
	if (!cmd->input_file)
		return (-1);
	*curr_token = (*curr_token)->next;
	return (0);
}

int	add_output_file(t_cmd *cmd, char *filename, int append_mode)
{
	char **new_files;
	int *new_modes;
	int i;

	new_files = malloc(sizeof(char *) * (cmd->output_count + 1));
	new_modes = malloc(sizeof(int) * (cmd->output_count + 1));
	if (!new_files || !new_modes)
	{
		if (new_files)
			free(new_files);
		if (new_modes)
			free(new_modes);
		return (-1);
	}
	i = 0;
	while (i < cmd->output_count)
	{
		new_files[i] = cmd->output_files[i];
		new_modes[i] = cmd->output_modes[i];
		i++;
	}
	new_files[cmd->output_count] = ft_strdup(filename);
	if (!new_files[cmd->output_count])
	{
		free(new_files);
		free(new_modes);
		return (-1);
	}
	new_modes[cmd->output_count] = append_mode;
	if (cmd->output_files)
		free(cmd->output_files);
	if (cmd->output_modes)
		free(cmd->output_modes);
	cmd->output_files = new_files;
	cmd->output_modes = new_modes;
	cmd->output_count++;
	return (0);
}

/*
 * Sets filename in output and append mode (if necessary) in cmd struct
 * @param cmd: command to modify
 * @param filename: filename to insert
 * @param append: 1 append mode, 0 overwrite mode
 * @param curr_token: token to skip
 * @return: 0 success, -1 error
 */
int	set_output_redirect(t_cmd *cmd, char *filename, int append, t_token **token)
{
	if (add_output_file(cmd, filename, append) == -1)
		return (-1);
	if (cmd->output_file)
		free(cmd->output_file);
	cmd->output_file = ft_strdup(filename);
	if (!cmd->output_file)
		return (-1);
	cmd->append_mode = append;
	*token = (*token)->next;
	return (0);
}

int add_heredoc_delimiter(t_cmd *cmd, char *delimiter)
{
	char	**new_array;
	int		i;

	new_array = malloc(sizeof(char *) * (cmd->heredoc_count + 2));
	if (!new_array)
		return (-1);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		new_array[i] = cmd->heredoc_delimiters[i];
		i++;
	}
	new_array[cmd->heredoc_count] = ft_strdup(delimiter);
	if (!new_array[cmd->heredoc_count])
	{
		free(new_array);
		return (-1);
	}
	new_array[cmd->heredoc_count + 1] = NULL;
	if (cmd->heredoc_delimiters)
		free(cmd->heredoc_delimiters);
	cmd->heredoc_delimiters = new_array;
	cmd->heredoc_count++;
	return (0);
}

/*
 * Sets heredoc delimiter in cmd struct
 * @param cmd: command to modify
 * @param delimiter: heredoc delimiter to set in cmd struct
 * @param curr_token: token to skip
 * @return: 0 success, -1 error
 */
int	set_heredoc_delimiter(t_cmd *cmd, char *delimiter, t_token **curr_token)
{
	if (add_heredoc_delimiter(cmd, delimiter) == -1)
		return (-1);
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	cmd->is_heredoc = 1;
	cmd->heredoc_delimiter = ft_strdup(delimiter);
	if (!cmd->heredoc_delimiter)
		return (-1);
	*curr_token = (*curr_token)->next;
	return (0);
}
