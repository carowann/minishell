/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:52:46 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/04 10:09:07 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: fix echo > > test should return error
// === PARSING RESULT ===
// === TOKEN LIST (4 tokens) ===
// [0] Type: WORD, Content: "echo"
// [1] Type: REDIRECT_OUT, Content: ">"
// [2] Type: REDIRECT_OUT, Content: ">"
// [3] Type: WORD, Content: "test"
// === END TOKEN LIST ===

// === DETAILED COMMAND LIST (1 commands) ===
// === COMMAND 0 DETAILS ===
// Arguments (2): "echo" "test" 
// Input: STDIN
// Output: FILE ">" (OVERWRITE)
// === END COMMAND 0 ===


/*
 * Parses input string into command list ready for execution
 * Handles tokenization internally using context pattern
 * @param input: command string to parse
 * @param cmd_list: output command list (TODO: implement)
 * @return: 0 success, -1 error
 */
int	parse_input(char *input, t_cmd_list	**cmd_list, t_shell_state **shell)
{
	t_tokenizer_ctx	ctx;

	if (init_and_tokenize(input, &ctx) == -1)
		return (-1);
	if (expand_variables(*shell, ctx.tokens) == -1)
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
 * Processes any remaining content in buffer at end of tokenization
 * Handles unclosed quotes as syntax errors, creates final tokens
 * Called after all input characters have been processed
 * @param ctx: tokenizer context with potential pending content
 * @return: 0 on success, -1 on syntax error (unclosed quotes)
 */
int finalize_pending_token(t_tokenizer_ctx *ctx)
{
	if (ctx->parser.state == IN_DOUBLE_QUOTES || ctx->parser.state == IN_SINGLE_QUOTES)
	{
		ft_putstr_fd("Syntax error: unclosed quotes\n", STDERR_FILENO);
		return (-1);
	}
	if (ctx->parser.buffer_pos > 0)
	{
		if (ctx->parser.state == IN_VARIABLE)
			return (safe_create_and_add_token(ctx, VARIABLE));
		else if (ctx->parser.state == IN_OPERATOR)
			return (create_redirect_token(ctx));
		else
			return (safe_create_and_add_token(ctx, WORD));
	}
	if (last_token_is_pipe(ctx->tokens))
	{
		ft_putstr_fd("Syntax error: bad pipe usage\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
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
	if (!*cmd_list)
	{
		cleanup_tokenizer_ctx(ctx);
		return (-1);
	}
	if (tokens_to_commands(ctx->tokens, *cmd_list) == -1)
	{
		free_command_list(*cmd_list);
		*cmd_list = NULL;
		cleanup_tokenizer_ctx(ctx);
		return (-1);
	}
	return (0);
}
