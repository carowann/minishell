/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine_extra.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:08:28 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/23 12:25:44 by cwannhed         ###   ########.fr       */
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
		ctx->parser.state = DEFAULT;
		return (safe_create_and_add_token(ctx, HEREDOC));
	}
	if (ctx->parser.buffer[0] == '>' && c == '>')
	{
		add_to_buffer(c, &ctx->parser);
		ctx->parser.state = DEFAULT;
		return (safe_create_and_add_token(ctx, APPEND));
	}
	ctx->parser.state = DEFAULT;
	if (ctx->parser.buffer[0] == '>' || ctx->parser.buffer[0] == '<')
	{
		if (create_redirect_token(ctx) == -1)
			return (-1);
	}
	return (0);
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

/*
 * Handles first character after $ in variable parsing
 * Manages special cases: $? (exit status), $123 (positional), $VAR (named)
 * @param c: first character after $
 * @param ctx: tokenizer context
 * @return: 0 on success, -1 on error
 */
int	handle_first_var_char(char c, t_tokenizer_ctx *ctx)
{
	if (c == '?')
	{
		add_to_buffer(c, &ctx->parser);
		if (safe_create_and_add_token(ctx, VARIABLE) == -1)
			return (-1);
		ctx->parser.state = DEFAULT;
	}
	else if (ft_isalpha(c) || c == '_')
		add_to_buffer(c, &ctx->parser);
	else if (ft_isdigit(c))
	{
		add_to_buffer(c, &ctx->parser);
		if (safe_create_and_add_token(ctx, VARIABLE) == -1)
			return (-1);
		ctx->parser.state = DEFAULT;
	}
	else
	{
		add_to_buffer('$', &ctx->parser);
		if (safe_create_and_add_token(ctx, WORD) == -1)
			return (-1);
		ctx->parser.state = DEFAULT;
		return (handle_state_machine(c, ctx));
	}
	return (0);
}

/*
 * Handles continuation characters in variable name parsing
 * Accepts alphanumeric characters and underscore for variable names
 * @param c: character to process
 * @param ctx: tokenizer context
 * @return: 0 on success, -1 on error
 */
int	handle_more_var_char(char c, t_tokenizer_ctx *ctx)
{
	if (ft_isalnum(c) || c == '_')
		add_to_buffer(c, &ctx->parser);
	else
	{
		if (safe_create_and_add_token(ctx, VARIABLE) == -1)
			return (-1);
		ctx->parser.state = DEFAULT;
		return (handle_state_machine(c, ctx));
	}
	return (0);
}
