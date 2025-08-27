/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:52:46 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/27 17:11:37 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Parses input string into command list ready for execution
 * Handles tokenization internally using context pattern
 * @param input: command string to parse
 * @param cmd_list: output command list (TODO: implement)
 * @return: 0 success, -1 error
 */
int	parse_input(char *input, t_cmd_list	**cmd_list, t_env **env)
{
	t_tokenizer_ctx	ctx;

	if (init_and_tokenize(input, &ctx) == -1)
		return (-1);
	if (expand_variables(*env, ctx.tokens) == -1)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	if (merge_adjacent_tokens(&ctx) == -1)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	if (build_cmd_list(cmd_list,&ctx) == -1)
		return (-1);
	printf("=== PARSING RESULT ===\n"); //debug
	print_token_list(ctx.tokens); //debug
	print_cmd_list_detailed(*cmd_list);  //debug
	cleanup_tokenizer_ctx(&ctx);
	return (0);
}

/*
 * Initialises tokenizer context and tokenizes input
 * @param input: input string to parse
 * @param ctx: tokenizer context
 * @return: 0 success, -1 error
 */
int	init_and_tokenize(char *input, t_tokenizer_ctx *ctx)
{
	if (init_tokenizer_ctx(ctx, input) == -1)
		return (-1);
	if (tokenize(input, ctx) == -1)
	{
		cleanup_tokenizer_ctx(ctx);
		return (-1);
	}
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

/*
 * Allocates command list and builds single commands through tokens
 * @param cmd_list: command list to populate
 * @param ctx: tokenizer context
 * @return: 0 success, -1 error
 */
int	build_cmd_list(t_cmd_list **cmd_list, t_tokenizer_ctx *ctx)
{
	*cmd_list = ft_calloc(1, sizeof(t_cmd_list));
	if (!cmd_list)
	{
		cleanup_tokenizer_ctx(ctx);
		return (-1);
	}
	if (tokens_to_commands(ctx->tokens, *cmd_list) == -1)
	{
		free_command_list(*cmd_list);
		cmd_list = NULL;
		cleanup_tokenizer_ctx(ctx);
		return (-1);
	}
	return (0);
}
