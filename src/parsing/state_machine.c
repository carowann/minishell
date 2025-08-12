/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:07:47 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/12 20:05:33 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_state_machine(char c, t_parser *parser)
{
	if (parser->state == DEFAULT)
		handle_default_state(c, parser);
	else if (parser->state == IN_DOUBLE_QUOTES)
		handle_double_quotes(c, parser);
	else if (parser->state == IN_SINGLE_QUOTES)
		handle_single_quotes(c, parser);
	else if (parser->state == IN_VARIABLE)
		handle_variable_state(c, parser);
	else if (parser->state == IN_OPERATOR)
		handle_operator_state(c, parser);
}

void handle_default_state(char c, t_parser *parser)
{
	if (c = '"')
	{
		parser->state = IN_DOUBLE_QUOTES;
		//buffer
	}
	else if (c == '\'')
	{	
		//
	}
	else if (c == '$')
	{
		parser->state = IN_VARIABLE;
		//buffer
	}
	//ecc
}
