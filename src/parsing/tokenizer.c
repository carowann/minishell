/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/14 14:40:54 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Tokenizes input using finite state machine
 * @param input: command string to parse
 * @param tokens: output token list
 * @return: 0 success, -1 error
 */
int	tokenize(char *input, t_token_list **tokens)
{
	t_state		state;
	char		*buffer;
	t_parser	parser;
	
	if (!input || !tokens)
		return (-1);
	*tokens = ft_calloc(1, sizeof(t_token_list));
	if (!*tokens)
		return (-1);
	buffer = ft_calloc(1, sizeof(ft_strlen(input) + 1));
	if (!buffer)
	{
		free_token_list(*tokens);
		return (-1);
	}
	**tokens = (t_token_list){0};
	parser = init_parser(parser, buffer, *tokens);
	state = DEFAULT;
	while (*input)
	{
		if (handle_state_machine(*input, &parser) == -1)
		{
			free_token_list(*tokens);
			return (-1);
		}
		input++;
	}
	free(buffer);
	return (0);
}

/*
 * Initializes token with type and content
 * @param token: pre-allocated token to fill
 * @param buffer: content source for WORD types  
 * @param type: token type (WORD, PIPE, etc.)
 * @return: 0 success, -1 on ft_strdup failure
 */
int	create_token(t_token *token, char	*buffer, t_token_type type)
{
	token->type = type;
	token->next = NULL;
	if (type == PIPE)
		token->content = ft_strdup("|");
	else if (type == REDIRECT_IN)
		token->content = ft_strdup("<");
	else if (type == REDIRECT_OUT)
		token->content = ft_strdup(">");
	else if (type == APPEND)
		token->content = ft_strdup(">>");
	else if (type ==HEREDOC)
		token->content = ft_strdup("<<");
	else
		token->content = ft_strdup(buffer);
	if (!token->content)
		return (-1);
	return (0);
}

/*
 * Creates and adds token, with buffer validation
 * PIPE tokens bypass buffer check, others require non-empty buffer
 * @param parser: parser state with buffer and token list
 * @param type: token type to create
 * @return: 0 success, -1 error
 */
int safe_create_and_add_token(t_parser *parser, t_token_type type)
{
	if (type == PIPE)
		return (create_and_add_token(parser, type));
	else
	{
		if (parser->buffer_pos == 0)
			return (0);
		if (create_and_add_token(parser, type) == -1)
			return (-1);
		reset_buffer(parser);
	}
	return (0);	
}

/*
 * Creates token and adds it to parser's token list
 * @param parser: parser with target token list
 * @param type: token type to create
 * @return: 0 success, -1 allocation error
 */
int	create_and_add_token(t_parser *parser, t_token_type	type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (-1);
	if (create_token(token, parser->buffer, type) == -1)
	{
		free(token);
		return (-1);
	}
	add_token_list(parser->tokens, token);
	return (0);
}

/*
 * Appends token to end of token list
 * @param token_list: target list
 * @param token: token to append
 */
void	add_token_list(t_token_list *token_list, t_token *token)
{
	t_token	*curr_token;

	if (!token_list)
		return ;
	curr_token = token_list->head;
	if (!token_list->head)
		token_list->head = token;
	else
	{
		while (curr_token->next)
			curr_token = curr_token->next;
		curr_token->next = token;
	}
	token_list->count++;
}
