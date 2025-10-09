/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:45:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/09 18:07:30 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Frees single token and its content
 * @param token: token to free (can be NULL)
 */
void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->content)
		free(token->content);
	free(token);
	token = NULL;
}

/*
 * Frees entire token list and all tokens
 * @param token_list: list to free (can be NULL)
 */
void	free_token_list(t_token_list *token_list)
{
	t_token	*curr_token;
	t_token	*temp_token;

	if (!token_list)
		return ;
	curr_token = token_list->head;
	while (curr_token)
	{
		temp_token = curr_token->next;
		free_token(curr_token);
		curr_token = temp_token;
	}
	free(token_list);
}

/*
 * Frees an array of strings and the array itself
 * @param array: array of strings to free (can be NULL)
 * @param count: number of elements in the array
 */
void	free_string_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
 * Cleans up all resources in tokenizer context
 * Frees token list, buffer, and zeros context
 * @param ctx: context to cleanup (can be NULL)
 */
void	cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx)
{
	if (ctx->tokens)
		free_token_list(ctx->tokens);
	if (ctx->parser.buffer)
		free(ctx->parser.buffer);
	*ctx = (t_tokenizer_ctx){0};
}
