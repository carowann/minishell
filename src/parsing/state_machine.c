/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:07:47 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/17 20:17:58 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Dispatches character processing based on current parser state
 * @param c: character to process
 * @param ctx: tokenizer context with parser state and token list
 * @return: 0 success, -1 error from state handlers
 */
int handle_state_machine(char c, t_tokenizer_ctx *ctx)
{
	if (ctx->parser.state == DEFAULT)
		return (handle_default_state(c, ctx));
	else if (ctx->parser.state == IN_DOUBLE_QUOTES)
		return (handle_double_quotes(c, ctx)); //TODO handle_double_quotes
	else if (ctx->parser.state == IN_SINGLE_QUOTES)
		return (handle_single_quotes(c, ctx));
	else if (ctx->parser.state == IN_VARIABLE)
		return (handle_variable_state(c,  ctx)); //TODO handle_variable_state
	else if (ctx->parser.state == IN_OPERATOR)
		return (handle_operator_state(c, ctx));
	return (0);
}

/*
 * Handles character processing in DEFAULT state
 * Manages state transitions and token creation for quotes, operators, pipes
 * @param c: character to process
 * @param ctx: tokenizer context with parser state and token list
 * @return: 0 success, -1 error
 */
int	handle_default_state(char c, t_tokenizer_ctx *ctx)
{
	if (c == '"')
		ctx->parser.state = IN_DOUBLE_QUOTES;
	else if (c == '\'')
		ctx->parser.state = IN_SINGLE_QUOTES;
	else if (c == '$')
		ctx->parser.state = IN_VARIABLE;
	else if (ft_isspace(c))
	{
		if (safe_create_and_add_token(ctx, WORD) == -1)
			return (-1);
	}
	else if (c == '<' || c == '>')
	{
		if (handle_operator_state(c, ctx) == -1)
			return (-1);
		ctx->parser.state = IN_OPERATOR;
	}
	else if (c == '|')
	{
		if (safe_create_and_add_token(ctx, WORD) == -1)
			return (-1);
		if (safe_create_and_add_token(ctx, PIPE) == -1)
			return (-1);
	}
	else
	{
		ctx->parser.buffer[ctx->parser.buffer_pos] = c;
		ctx->parser.buffer_pos++;
	}
	return (0);
}

/*
 * Stub: handles character processing in double quotes
 * TODO: implement quote content accumulation and variable expansion
 * @param c: character to process
 * @param ctx: tokenizer context
 * @return: 0 (always success for now)
 */
int	handle_double_quotes(char c, t_tokenizer_ctx *ctx)
{
	(void)c;
	(void)ctx;
	printf("TODO: handle_double_quotes called with '%c'\n", c);
	return (0);
}

/*
 * Handles character processing inside single quotes
 * Single quotes preserve all characters literally (no variable expansion)
 * Only the closing quote has special meaning
 * @param c: character to process
 * @param ctx: tokenizer context
 * @return: 0 on success, -1 on token creation failure
 */
int	handle_single_quotes(char c, t_tokenizer_ctx *ctx)
{
	if (c == '\'')
	{
		if (safe_create_and_add_token(ctx, WORD) == -1)
			return (-1);
		ctx->parser.state = DEFAULT;
	}
	else
	{
		ctx->parser.buffer[ctx->parser.buffer_pos] = c;
		ctx->parser.buffer_pos++;
	}
	return (0);
}

/*
 * Handles operator parsing with support for both single and double operators
 * Recognizes: < > << >> and creates appropriate redirect tokens
 * Uses buffer to accumulate operator characters before deciding token type
 * @param c: character to process
 * @param ctx: tokenizer context
 * @return: 0 on success, -1 on error
 */
int	handle_operator_state(char c, t_tokenizer_ctx *ctx)
{
	if (ctx->parser.buffer_pos == 0)
	{
		add_to_buffer(c, &ctx->parser);
		return (0);
	}
	if (ctx->parser.buffer[0] == '<' && c == '<')
	{
		add_to_buffer(c, &ctx->parser);
		return (safe_create_and_add_token(ctx, HEREDOC));
	}
	if (ctx->parser.buffer[0] == '>' && c == '>')
	{
		add_to_buffer(c, &ctx->parser);
		return (safe_create_and_add_token(ctx, APPEND));
	}
	ctx->parser.state = DEFAULT;
	if (ctx->parser.buffer[0] == '>')
	{
		if (safe_create_and_add_token(ctx, REDIRECT_OUT) == -1)
			return (-1);		
	}
	if (ctx->parser.buffer[0] == '<')
	{
		if (safe_create_and_add_token(ctx, REDIRECT_IN) == -1)
			return (-1);		
	}
	return (handle_state_machine(c, ctx));
}
