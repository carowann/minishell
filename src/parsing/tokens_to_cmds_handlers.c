/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_cmds_handlers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 12:54:25 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/23 16:47:55 by lzorzit          ###   ########.fr       */
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
		ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `|'\n");
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
int	handle_redirect_token(t_token **curr_token, t_cmd *cmd)
{
	t_token	*filename_token;
	
	if (!curr_token || !*curr_token || !cmd)
		return (-1);
	filename_token = (*curr_token)->next;
	if (!filename_token || filename_token->type == PIPE)
	{
		ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (is_redirect_token(filename_token))
	{
		ft_printfd(STDERR_FILENO, "minishell: syntax error near unexpected token `%s'\n", filename_token->content);
		return (-1);
	}
	if ((*curr_token)->type == REDIRECT_IN)
		return (set_input_redirect(cmd, (*filename_token).content, curr_token));
	else if ((*curr_token)->type == REDIRECT_OUT)
		return (set_output_redirect(cmd, (*filename_token).content, 0, curr_token));
	else if ((*curr_token)->type == APPEND)
		return (set_output_redirect(cmd, (*filename_token).content, 1, curr_token));
	else if ((*curr_token)->type == HEREDOC)
		return (set_heredoc_delimiter(cmd, (*filename_token).content, curr_token));
	return (-1);
}
