/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:52:46 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/18 17:16:01 by cwannhed         ###   ########.fr       */
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
int	parse_input(char *input, t_cmd_list	**commands)
{
	t_tokenizer_ctx	ctx;
	(void)commands;

	if (init_tokenizer_ctx(&ctx, input) == -1)
		return (-1);
	if (tokenize(input, &ctx) == -1)
	{
		cleanup_tokenizer_ctx(&ctx);
		return (-1);
	}
	// if (expand_variables(ctx.tokens) == -1) //TODO: expand_variables
	// {
	// 	cleanup_tokenizer_ctx(&ctx);
	// 	return (-1);
	// }
	// *commands = (t_cmd_list *){0};
	// if (tokens_to_commands(ctx.tokens, commands) == -1)
	// {
	// 	cleanup_tokenizer_ctx(&ctx);
	// 	return (-1);
	// }
	printf("Tokens creati: %d\n", ctx.tokens->count);
	print_token_list(ctx.tokens);
	cleanup_tokenizer_ctx(&ctx);
	return (0);
}
