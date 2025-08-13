/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/13 18:02:09 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//token list: struttura temportanea per i token
//command list: struttura finale per execution
//dopo il parsing, token list viene distrutta e command list viene usata per l'esecuzione

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
