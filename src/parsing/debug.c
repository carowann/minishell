/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:02:52 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/19 15:06:29 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void print_token_list(t_token_list *tokens)
{
	t_token	*current;
	int		i;

	i = 0;
	if (!tokens || !tokens->head)
	{
		printf("Token list is empty\n");
		return ;
	}
	printf("=== TOKEN LIST (%d tokens) ===\n", tokens->count);
	current = tokens->head;
	while (current)
	{
		printf("[%d] Type: %s, Content: \"%s\"\n",
				i , get_token_type_name(current->type), current->content);
		current = current->next;
		i++;
	}
	printf("=== END TOKEN LIST ===\n\n");
}

const char *get_token_type_name(t_token_type type)
{
	switch (type)
	{
	case WORD:
		return "WORD";
	case PIPE:
		return "PIPE";
	case REDIRECT_IN:
		return "REDIRECT_IN";
	case REDIRECT_OUT:
		return "REDIRECT_OUT";
	case APPEND:
		return "APPEND";
	case HEREDOC:
		return "HEREDOC";
	case QUOTED_STRING:
		return "QUOTED_STRING";
	case VARIABLE:
		return "VARIABLE";
	default:
		return "UNKNOWN";
	}
}

void	print_cmd_list(t_cmd_list *cmd_list)
{
	t_cmd	*curr_cmd;
	int		cmd_num;
	int		i;

	cmd_num = 0;
	if (!cmd_list || !cmd_list->head)
	{
		printf("Command list is empty\n");
		return ;
	}
	printf("=== COMMAND LIST (%d commands) ===\n", cmd_list->count);
	curr_cmd = cmd_list->head;
	while (curr_cmd)
	{
		printf("[CMD %d] args: ", cmd_num);
		if (curr_cmd->args)
		{
			i = 0;
			while (curr_cmd->args[i])
			{
				printf("\"%s\" ", curr_cmd->args[i]);
				i++;
			}
		}
		printf("(count: %d)\n", curr_cmd->arg_count);
		curr_cmd = curr_cmd->next;
		cmd_num++;
	}
	printf("=== END COMMAND LIST ===\n\n");
}
