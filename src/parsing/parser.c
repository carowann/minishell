/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:52:46 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/22 17:53:50 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Parses input string into command list ready for execution
 * Handles tokenization internally using context pattern
 * @param input: command string to parse
 * @param commands: output command list (TODO: implement)
 * @return: 0 success, -1 error
 */
int	parse_input(char *input, t_cmd_list	**commands, t_env **env)
{
	t_tokenizer_ctx	ctx;

	if (init_tokenizer_ctx(&ctx, input) == -1)
		return (-1);
	if (tokenize(input, &ctx) == -1)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	if (expand_variables(*env, ctx.tokens) == -1)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	*commands = ft_calloc(1, sizeof(t_cmd_list));
	if (!*commands)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	if (tokens_to_commands(ctx.tokens, *commands) == -1)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	printf("=== PARSING RESULT ===\n"); //debug
	print_token_list(ctx.tokens); //debug
	print_cmd_list(*commands); //debug
	cleanup_tokenizer_ctx(&ctx);
	return (0);
}

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
	return (finalize_pending_token(ctx));
}
