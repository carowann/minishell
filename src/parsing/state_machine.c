/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_machine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 19:07:47 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/12 19:15:00 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_state_machine(char c, t_state *state, char *buffer, t_token_list *tokens)
{
	if (state == DEFAULT)
		handle_default_state(c, state, buffer, tokens);
	else if (state == IN_DOUBLE_QUOTES)
		handle_double_quotes(c, state, buffer, tokens);
	else if (state == IN_SINGLE_QUOTES)
		handle_single_quotes(c, state, buffer, tokens);
	else if (state == IN_VARIABLE)
		handle_variable_state(c, state, buffer, tokens);
	else if (state == IN_OPERATOR)
		handle_operator_state(c, state, buffer, tokens);
}
