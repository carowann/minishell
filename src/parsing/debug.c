/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:02:52 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/14 19:58:59 by cwannhed         ###   ########.fr       */
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
		printf("[%d] Type: %d, Content: \"%s\"\n",
				i , current->type, current->content);
		current = current->next;
		i++;
	}
	printf("=== END TOKEN LIST ===\n");
}
