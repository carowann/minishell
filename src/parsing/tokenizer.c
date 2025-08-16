/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/16 19:22:48 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Tokenizes input using finite state machine with context
 * @param input: command string to parse
 * @param ctx: tokenizer context (pre-initialized)
 * @return: 0 success, -1 error
 */
int	tokenize(char *input, t_tokenizer_ctx *ctx)
{	
	if (!input || !ctx)
		return (-1);
	while (*input)
	{
		if (handle_state_machine(*input, ctx) == -1)
			return (-1);
		input++;
	}
	if (ctx->parser.state != DEFAULT) //left open quotes
		return (-1);
	if (ctx->parser.buffer_pos > 0)
	{
		if (safe_create_and_add_token(ctx, WORD) == -1)
			return (-1);
	}
	return (0);
}

/*
 * Initializes token with type and content
 * @param token: pre-allocated token to fill
 * @param buffer: content source for WORD types  
 * @param type: token type (WORD, PIPE, etc.)
 * @return: 0 success, -1 on ft_strdup failure
 */
int	create_token(t_token *token, char *buffer, t_token_type type)
{
	token->type = type;
	token->next = NULL;
	if (type == PIPE)
		token->content = ft_strdup("|");
	else if (type == REDIRECT_IN)
		token->content = ft_strdup("<");
	else if (type == REDIRECT_OUT)
		token->content = ft_strdup(">");
	else if (type == APPEND)
		token->content = ft_strdup(">>");
	else if (type ==HEREDOC)
		token->content = ft_strdup("<<");
	else
		token->content = ft_strdup(buffer);
	if (!token->content)
		return (-1);
	return (0);
}

/*
 * Creates and adds token with buffer validation
 * PIPE tokens bypass buffer check, others require non-empty buffer
 * @param ctx: tokenizer context with parser state and token list
 * @param type: token type to create
 * @return: 0 success, -1 error
 */
int safe_create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type)
{
	if (type == PIPE)
		return (create_and_add_token(ctx, type));
	else
	{
		if (ctx->parser.buffer_pos == 0)
			return (0);
		if (create_and_add_token(ctx, type) == -1)
			return (-1);
		reset_buffer(&ctx->parser);
	}
	return (0);	
}

/*
 * Creates token and adds it to context token list
 * @param ctx: tokenizer context
 * @param type: token type to create
 * @return: 0 success, -1 allocation error
 */
int	create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (-1);
	if (create_token(token, ctx->parser.buffer, type) == -1)
	{
		free(token);
		return (-1);
	}
	add_token_list(ctx->tokens, token);
	return (0);
}

/*
 * Appends token to end of token list
 * @param token_list: target list
 * @param token: token to append
 */
void	add_token_list(t_token_list *token_list, t_token *token)
{
	t_token	*curr_token;

	if (!token_list)
		return ;
	curr_token = token_list->head;
	if (!token_list->head)
		token_list->head = token;
	else
	{
		while (curr_token->next)
			curr_token = curr_token->next;
		curr_token->next = token;
	}
	token_list->count++;
}
