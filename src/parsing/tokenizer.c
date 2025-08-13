/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/13 16:56:32 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//token list: struttura temportanea per i token
//command list: struttura finale per execution
//dopo il parsing, token list viene distrutta e command list viene usata per l'esecuzione

static t_parser init_parser(t_parser parser, char *buffer, t_token_list *tokens)
{
	parser.buffer = buffer;
	parser.state = DEFAULT;
	parser.buffer_pos = 0;
	parser.tokens = tokens;
	return (parser);
}

int	tokenize(char *input,  t_token_list **tokens)
{
	t_token_list	*temp_tokens;
	t_state			state;
	char			*buffer;
	t_parser		parser;
	
	temp_tokens = ft_calloc(1, sizeof(t_token_list));
	if (!temp_tokens)
		return (-1);
	buffer = ft_calloc(1, sizeof(ft_strlen(input) + 1));
	if (!buffer)
	{
		free_token_list(temp_tokens);
		return (-1);		
	}
	*temp_tokens = (t_token_list){0};
	parser = init_parser(parser, buffer, temp_tokens);
	state = DEFAULT;
	while (*input)
	{
		if (handle_state_machine(*input, &parser) == -1)
		{
			free_token_list(temp_tokens);
			return (-1);
		}
		input++;
	}
	*tokens = temp_tokens;
	free(buffer);
	return (0);
}

t_token	*create_token(t_parser *parser, t_token_type type)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		handle_error(); //TODO handle_error
	token->type = type;
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
		token->content = ft_strdup(parser->buffer);
	token->next = NULL;
	return (token);
}

int safe_create_and_add_token(t_parser *parser, t_token_type type)
{
	if (type == PIPE)
	{
		return (create_and_add_token(parser, type));
	}
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
