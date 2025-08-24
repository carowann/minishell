/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_operators.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:43:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/24 18:59:53 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	set_output_redirect(t_cmd *cmd, char *filename, int append, t_token **curr_token)
{
	if (cmd->output_file)
		free(cmd->output_file);
	cmd->append_mode = append;
	cmd->output_file = ft_strdup(filename);
	if (!cmd->output_file)
		return (-1);
	*curr_token = (*curr_token)->next;
	return (0);
}

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
