/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_operators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:43:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/24 17:30:42 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Sets filename in input in cmd struct
 * @param cmd: command to modify
 * @param filename: filename to insert
 * @param curr_token: token to skip
 * @return: 0 success, -1 error
 */
int	set_input_redirect(t_cmd *cmd, char *filename, t_token **curr_token)
{
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(filename);
	if (!cmd->input_file)
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
int	set_output_redirect(t_cmd *cmd, char *filename, int append, t_token **token)
{
	if (cmd->output_file)
		free(cmd->output_file);
	cmd->append_mode = append;
	cmd->output_file = ft_strdup(filename);
	if (!cmd->output_file)
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
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	cmd->is_heredoc = 1;
	cmd->heredoc_delimiter = ft_strdup(delimiter);
	if (!cmd->heredoc_delimiter)
		return (-1);
	*curr_token = (*curr_token)->next;
	return (0);
}
