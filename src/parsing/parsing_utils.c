/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:03:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/13 17:35:23 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_parser init_parser(t_parser parser, char *buffer, t_token_list *tokens)
{
	parser.buffer = buffer;
	parser.state = DEFAULT;
	parser.buffer_pos = 0;
	parser.tokens = tokens;
	return (parser);
}

void	reset_buffer(t_parser *parser)
{
	parser->buffer[0] = '\0';
	parser->buffer_pos= 0;
}
