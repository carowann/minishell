/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_operators_utils.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 18:38:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/08 16:10:20 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_input_file(t_cmd *cmd, char *filename)
{
	char	**new_array;
	int		i;

	new_array = ft_calloc(sizeof(char *), (cmd->input_count + 2));
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

int	add_output_file(t_cmd *cmd, char *filename, int append_mode)
{
	char	**new_files;
	int		*new_modes;

	if (allocate_output_arrays(cmd, &new_files, &new_modes) == -1)
		return (-1);
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

int	add_heredoc_delimiter(t_cmd *cmd, char *delimiter)
{
	char	**new_array;
	int		i;

	new_array = ft_calloc(sizeof(char *), (cmd->heredoc_count + 2));
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
	if (cmd->heredoc_delimiters)
		free(cmd->heredoc_delimiters);
	cmd->heredoc_delimiters = new_array;
	cmd->heredoc_count++;
	return (0);
}

int	allocate_output_arrays(t_cmd *cmd, char ***new_files, int **new_modes)
{
	int	i;

	*new_files = ft_calloc(sizeof(char *), (cmd->output_count + 2));
	*new_modes = ft_calloc(sizeof(int), (cmd->output_count + 2));
	if (!*new_files || !*new_modes)
	{
		if (*new_files)
			free(*new_files);
		if (*new_modes)
			free(*new_modes);
		return (-1);
	}
	i = 0;
	while (i < cmd->output_count)
	{
		(*new_files)[i] = cmd->output_files[i];
		(*new_modes)[i] = cmd->output_modes[i];
		i++;
	}
	return (0);
}
