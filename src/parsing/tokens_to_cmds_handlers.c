/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:54:25 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/26 10:38:10 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Adds argument to array of arguments of command
 * @param token: token w/ argument to append to array
 * @param cmd: command to update with new argument
 * @return: 0 success, -1 error
 */
int	handle_argument_token(t_token *token, t_cmd *cmd)
{
	char	**new_args;
	int		i;

	if (!token->content || !cmd)
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
	new_args[cmd->arg_count] = ft_strdup(token->content);
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
 * Adds commands to command list and creates new command
 * @param cmd: command to add
 * @param cmd_list: list of commands
 * @return: 0 success, -1 error
 */
int	handle_pipe_token(t_cmd **cmd, t_cmd_list *cmd_list)
{
	if (!(*cmd)->arg_count && !(*cmd)->input_file)
	{
		ft_printfd(2, "minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (add_command_to_list(*cmd, cmd_list) == -1)
	{
		free_cmd(*cmd);
		return (-1);
	}
	*cmd = ft_calloc(1, sizeof(t_cmd));
	if (!*cmd)
		return (-1);
	return (0);
}

/*
 * Analyses type of redirection and handles relative token
 * @param curr_token: token to analyse
 * @param cmd: command result of conversion
 * @return: 0 success, -1 error
 */
int	handle_redirect_token(t_token **token, t_cmd *cmd)
{
	t_token	*filename_token;

	if (!token || !*token || !cmd)
		return (-1);
	filename_token = (*token)->next;
	if (!filename_token || filename_token->type == PIPE)
	{
		ft_printfd(2, "minishell: syntax error\n");
		return (-1);
	}
	if (is_redirect_token(filename_token))
	{
		ft_printfd(2, "minishell: syntax error\n");
		return (-1);
	}
	if ((*token)->type == REDIRECT_IN)
		return (set_input_redirect(cmd, (*filename_token).content, token));
	else if ((*token)->type == REDIRECT_OUT)
		return (set_output_redirect(cmd, (*filename_token).content, 0, token));
	else if ((*token)->type == APPEND)
		return (set_output_redirect(cmd, (*filename_token).content, 1, token));
	else if ((*token)->type == HEREDOC)
		return (set_heredoc_delimiter(cmd, (*filename_token).content, token));
	return (-1);
}
