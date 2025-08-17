/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:02:52 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/17 17:47:55 by cwannhed         ###   ########.fr       */
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
	printf("=== END TOKEN LIST ===\n");
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
