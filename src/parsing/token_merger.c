/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_merger.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 16:27:26 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/26 18:09:33 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	merge_adjacent_tokens(t_tokenizer_ctx *ctx)
{
	t_token	*curr_token;

	if (!ctx || !ctx->tokens || !ctx->tokens->head)
		return (-1);
	curr_token = ctx->tokens->head;
	while (curr_token && curr_token->next)
	{
		if (should_merge_tokens(curr_token, curr_token->next))
		{
			if (merge_tokens(curr_token, curr_token->next) == -1)
				return (-1);
			ctx->tokens->count--;
		}
		else
			curr_token = curr_token->next;
	}
	return (0);
}

int	should_merge_tokens(t_token *curr, t_token *next)
{
	if (next->prec_space
		|| curr->type == PIPE || next->type == PIPE
		|| is_redirect_token(curr) || is_redirect_token(next))
		return (0);
	return (is_argument_token(curr) && is_argument_token(next));
}

int	merge_tokens(t_token *curr, t_token *next)
{
	char	*merged_content;
	t_token	*temp;
	
	if (!curr || !next)
		return (-1);
	merged_content = ft_strjoin(curr->content, next->content);
	if (!merged_content)
		return (-1);
	free(curr->content);
	curr->content = merged_content;
	temp = curr->next;
	curr->next = next->next;
	free(temp);
	return (0);
}
