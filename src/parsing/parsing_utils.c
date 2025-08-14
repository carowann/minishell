/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:03:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/14 16:31:36 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Clears parser buffer and resets position to start
 * @param parser: parser with buffer to reset
 */
void reset_buffer(t_parser *parser)
{
	parser->buffer[0] = '\0';
	parser->buffer_pos= 0;
}

int	init_tokenizer_ctx(t_tokenizer_ctx *ctx, char *input)
{
	*ctx = (t_tokenizer_ctx){0};
	ctx->tokens = ft_calloc(1, sizeof(t_token_list));
	if (!ctx->tokens)
		return (-1);
	*ctx->tokens = (t_token_list){0};
	ctx->parser = (t_parser){0};
	ctx->parser.buffer = ft_calloc(1, ft_strlen(input) + 1);
	if (!ctx->parser.buffer)
	{
		free_token_list(ctx->tokens);
		return (-1);
	}
	ctx->parser.state = DEFAULT;
	return (0);
}

void cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx)
{
	if (ctx->tokens)
		free_token_list(ctx->tokens);
	if (ctx->parser.buffer)
		free(ctx->parser.buffer);
	*ctx = (t_tokenizer_ctx){0};
}
