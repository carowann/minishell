/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:07:47 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/13 16:47:01 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_state_machine(char c, t_parser *parser)
{
	if (parser->state == DEFAULT)
		return (handle_default_state(c, parser));
	else if (parser->state == IN_DOUBLE_QUOTES)
		return (handle_double_quotes(c, parser)); //TODO handle_double_quotes
	else if (parser->state == IN_SINGLE_QUOTES)
		return (handle_single_quotes(c, parser)); //TODO handle_single_quotes
	else if (parser->state == IN_VARIABLE)
		return (handle_variable_state(c, parser)); //TODO handle_variable_state
	else if (parser->state == IN_OPERATOR)
		return (handle_operator_state(c, parser)); //TODO handle_operator_state
	return (0);
}

int handle_default_state(char c, t_parser *parser)
{
	if (c == '"')
		parser->state = IN_DOUBLE_QUOTES;
	else if (c == '\'')
		parser->state = IN_SINGLE_QUOTES;
	else if (c == '$')
		parser->state = IN_VARIABLE;
	else if (ft_isspace(c))
	{
		if (safe_create_and_add_token(parser, WORD) == -1)
			return (-1);
	}
	else if (c == '<' || c =='>')
	{
		if (safe_create_and_add_token(parser, WORD) == -1)
			return (-1);
		parser->state = IN_OPERATOR;
	}
	else if (c == '|')
	{
		if (safe_create_and_add_token(parser, WORD) == -1)
			return (-1);
		if (safe_create_and_add_token(parser, PIPE) == -1)
			return (-1);
	}
	else
	{
		parser->buffer[parser->buffer_pos] = c;
		parser->buffer_pos++;
	}
	return (0);
}

