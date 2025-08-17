/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:03:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/17 20:17:27 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Safely appends character to parser buffer
 * Automatically null-terminates string after addition
 * @param c: character to append to buffer
 * @param parser: parser context containing buffer and position
 */
void	add_to_buffer(char c, t_parser *parser)
{
	parser->buffer[parser->buffer_pos] = c;
	parser->buffer_pos++;
	parser->buffer[parser->buffer_pos] = '\0';
}

/*
 * Clears parser buffer and resets position to start
 * @param parser: parser with buffer to reset
 */
void reset_buffer(t_parser *parser)
{
	ft_bzero(parser->buffer, ft_strlen(parser->buffer));
	parser->buffer_pos = 0;
}

/*
 * Initializes tokenizer context with input string
 * Allocates token list and buffer, sets up parser state
 * @param ctx: context to initialize
 * @param input: string to tokenize
 * @return: 0 success, -1 allocation error
 */
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

/*
 * Cleans up all resources in tokenizer context
 * Frees token list, buffer, and zeros context
 * @param ctx: context to cleanup (can be NULL)
 */
void cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx)
{
	if (ctx->tokens)
		free_token_list(ctx->tokens);
	if (ctx->parser.buffer)
		free(ctx->parser.buffer);
	*ctx = (t_tokenizer_ctx){0};
}
