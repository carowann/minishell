/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:07:47 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/13 13:20:21 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_state_machine(char c, t_parser *parser)
{
	if (parser->state == DEFAULT)
		handle_default_state(c, parser);
	else if (parser->state == IN_DOUBLE_QUOTES)
		handle_double_quotes(c, parser); //TODO
	else if (parser->state == IN_SINGLE_QUOTES)
		handle_single_quotes(c, parser); //TODO
	else if (parser->state == IN_VARIABLE)
		handle_variable_state(c, parser); //TODO
	else if (parser->state == IN_OPERATOR)
		handle_operator_state(c, parser); //TODO
}

void handle_default_state(char c, t_parser *parser)
{
	if (c == '"')
		parser->state = IN_DOUBLE_QUOTES;
	else if (c == '\'')
		parser->state = IN_SINGLE_QUOTES;
	else if (c == '$')
		parser->state = IN_VARIABLE;
	else if (ft_isspace(c))
	{
		if (parser->buffer_pos > 0)
		{
			create_and_add_token(parser); //TODO
			reset_buffer(parser); //TODO
		}
	}
	else if (c == '<' || c =='>')
	{
		if (parser->buffer_pos > 0)
		{
			create_and_add_token(parser);
			reset_buffer(parser);
		}
		parser->state = IN_OPERATOR;
	}
	else if (c == '|')
	{
		if (parser->buffer_pos > 0)
		{
			create_and_add_token(parser);
			reset_buffer(parser);
		}
		create_pipe_token(parser); //TODO
	}
	else



















	{
		parser->buffer[parser->buffer_pos] = c;
		parser->buffer_pos++;
	}
}
