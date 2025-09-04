/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine_handlers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 18:47:53 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/04 14:20:22 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_dollar_char(t_tokenizer_ctx *ctx)
{
	if (safe_create_and_add_token(ctx, WORD) == -1)
		return (-1);
	ctx->parser.state = IN_VARIABLE;
	return (0);
}

int	handle_space_char(t_tokenizer_ctx *ctx)
{
	if (safe_create_and_add_token(ctx, WORD) == -1)
		return (-1);
	ctx->parser.space_encountered = 1;
	return (0);
}

int	handle_redirect_start(char c, t_tokenizer_ctx *ctx)
{
	if (handle_operator_state(c, ctx) == -1)
		return (-1);
	ctx->parser.state = IN_OPERATOR;
	return (0);
}

int	handle_pipe_char(t_tokenizer_ctx *ctx)
{
	if (safe_create_and_add_token(ctx, WORD) == -1)
		return (-1);
	return (safe_create_and_add_token(ctx, PIPE));
}
