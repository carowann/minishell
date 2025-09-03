/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine_default.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 18:47:53 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/03 18:48:58 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_dollar_char(t_tokenizer_ctx *ctx);
static int	handle_space_char(t_tokenizer_ctx *ctx);
static int	handle_redirect_start(char c, t_tokenizer_ctx *ctx);
static int	handle_pipe_char(t_tokenizer_ctx *ctx);

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
		return (handle_dollar_char(ctx));
	else if (ft_isspace(c))
		return (handle_space_char(ctx));
	else if (c == '<' || c == '>')
		return (handle_redirect_start(c, ctx));
	else if (c == '|')
		return (handle_pipe_char(ctx));
	else
		add_to_buffer(c, &ctx->parser);
	return (0);
}

static int	handle_dollar_char(t_tokenizer_ctx *ctx)
{
	if (safe_create_and_add_token(ctx, WORD) == -1)
		return (-1);
	ctx->parser.state = IN_VARIABLE;
	return (0);
}

static int	handle_space_char(t_tokenizer_ctx *ctx)
{
	if (safe_create_and_add_token(ctx, WORD) == -1)
		return (-1);
	ctx->parser.space_encountered = 1;
	return (0);
}

static int	handle_redirect_start(char c, t_tokenizer_ctx *ctx)
{
	if (handle_operator_state(c, ctx) == -1)
		return (-1);
	ctx->parser.state = IN_OPERATOR;
	return (0);
}

static int	handle_pipe_char(t_tokenizer_ctx *ctx)
{
	if (safe_create_and_add_token(ctx, WORD) == -1)
		return (-1);
	return (safe_create_and_add_token(ctx, PIPE));
}
