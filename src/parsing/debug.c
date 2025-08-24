/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:02:52 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/24 19:20:21 by cwannhed         ###   ########.fr       */
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

void print_cmd_details(t_cmd *cmd, int cmd_num)
{
	int i;

	printf("=== COMMAND %d DETAILS ===\n", cmd_num);
	printf("Arguments (%d): ", cmd->arg_count);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("\"%s\" ", cmd->args[i]);
			i++;
		}
	}
	printf("\n");
	printf("Input: ");
	if (cmd->is_heredoc)
	{
		printf("HEREDOC (delimiter: \"");
		if (cmd->heredoc_delimiter)
			printf("%s", cmd->heredoc_delimiter);
		else
			printf("NULL");
		printf("\")\n");
	}
	else if (cmd->input_file)
		printf("FILE \"%s\"\n", cmd->input_file);
	else
		printf("STDIN\n");
	printf("Output: ");
	if (cmd->output_file)
	{
		printf("FILE \"%s\" (", cmd->output_file);
		if (cmd->append_mode)
			printf("APPEND");
		else
			printf("OVERWRITE");
		printf(")\n");
	}
	else
		printf("STDOUT\n");
	printf("=== END COMMAND %d ===\n\n", cmd_num);
}

void print_cmd_list_detailed(t_cmd_list *cmd_list)
{
	t_cmd *curr_cmd;
	int cmd_num;

	if (!cmd_list || !cmd_list->head)
	{
		printf("Command list is empty\n");
		return;
	}
	printf("=== DETAILED COMMAND LIST (%d commands) ===\n", cmd_list->count);
	curr_cmd = cmd_list->head;
	cmd_num = 0;
	while (curr_cmd)
	{
		print_cmd_details(curr_cmd, cmd_num);
		curr_cmd = curr_cmd->next;
		cmd_num++;
	}
}