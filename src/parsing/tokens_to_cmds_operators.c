/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_operators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:43:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/24 16:01:44 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
* Validates output redirect file before setting it
* Opens and immediately closes file to test permissions
* @param cmd: command structure to update
* @param filename: target file for redirection
* @param append: 1 for append mode, 0 for truncate
* @return: 0 success, -1 error with message printed
*/
int validate_and_set_output_redirect(t_cmd *cmd, char *filename, int append)
{
	char *file_path;
	char *last_slash;

	if (filename[0] == '/' && access(filename, F_OK) != 0)
	{
		file_path = ft_strdup(filename);
		if (!file_path)
			return (-1);
		last_slash = ft_strrchr(file_path, '/');
		if (last_slash)
		{
			*last_slash = '\0';
			if (access(file_path, F_OK) != 0)
			{
				ft_printfd(2, "minishell: %s: No such file or directory\n", filename);
				free(file_path);
				return (-1);
			}
		}
		free(file_path);
	}
	if (cmd->output_file)
		free(cmd->output_file);
	cmd->output_file = ft_strdup(filename);
	if (!cmd->output_file)
		return (-1);
	cmd->append_mode = append;
	return (0);
}

/*
 * Validates input redirect file before setting it
 * @param cmd: command structure to update
 * @param filename: source file for input redirection
 * @return: 0 success, -1 error with message printed
 */
int validate_and_set_input_redirect(t_cmd *cmd, char *filename)
{
	int test_fd;

	test_fd = open(filename, O_RDONLY);
	if (test_fd < 0)
	{
		ft_printfd(2, "minishell: %s: %s\n", filename, strerror(errno));
		return (-1);
	}
	close(test_fd);
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(filename);
	if (!cmd->input_file)
		return (-1);
	return (0);
}

/*
 * Sets filename in input in cmd struct
 * @param cmd: command to modify
 * @param filename: filename to insert
 * @param curr_token: token to skip
 * @return: 0 success, -1 error
 */
int set_input_redirect(t_cmd *cmd, char *filename, t_token **curr_token)
{
	if (validate_and_set_input_redirect(cmd, filename) == -1)
		return (-1);
	*curr_token = (*curr_token)->next;
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
int set_output_redirect(t_cmd *cmd, char *filename, int append, t_token **token)
{
	if (validate_and_set_output_redirect(cmd, filename, append) == -1)
		return (-1);
	*token = (*token)->next;
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
	char	**new_delimiters;
	int		i;

	if (!cmd || !delimiter)
		return (-1);
	new_delimiters = ft_calloc(cmd->heredoc_count + 2, sizeof(char *));
	if (!new_delimiters)
		return (-1);
	i = 0;
	while (i < cmd->heredoc_count)
	{
		new_delimiters[i] = cmd->heredoc_delimiters[i];
		i++;
	}
	new_delimiters[cmd->heredoc_count] = ft_strdup(delimiter);
	if (!new_delimiters[cmd->heredoc_count])
	{
		free(new_delimiters);
		return (-1);
	}
	new_delimiters[cmd->heredoc_count + 1] = NULL;
	if (cmd->heredoc_delimiters)
		free(cmd->heredoc_delimiters);
	cmd->heredoc_delimiters = new_delimiters;
	cmd->heredoc_count++;
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	cmd->heredoc_delimiter = ft_strdup(delimiter);
	cmd->is_heredoc = 1;
	*curr_token = (*curr_token)->next;
	return (0);
}
