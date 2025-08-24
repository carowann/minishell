/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_state_handler.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:34:55 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/24 19:05:51 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	if (ctx->parser.buffer[0] == '>' || ctx->parser.buffer[0] == '<')
	{
		if (create_redirect_token(ctx) == -1)
			return (-1);
	}
	return (handle_state_machine(c, ctx));
}

/*
 * Creates appropriate redirect tokens
 * @param ctx: tokenizer context
 * @return: 0 on success, -1 on error
 */
int	create_redirect_token(t_tokenizer_ctx *ctx)
{
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
	return (0);
}
