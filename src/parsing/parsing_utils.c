/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 13:03:31 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/14 14:40:43 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Initializes parser structure with buffer and token list
 * @param parser: parser struct to initialize
 * @param buffer: character buffer for token building
 * @param tokens: target token list
 * @return: initialized parser struct
 */
t_parser init_parser(t_parser parser, char *buffer, t_token_list *tokens)
{
	parser.buffer = buffer;
	parser.state = DEFAULT;
	parser.buffer_pos = 0;
	parser.tokens = tokens;
	return (parser);
}

/*
 * Clears parser buffer and resets position to start
 * @param parser: parser with buffer to reset
 */
void	reset_buffer(t_parser *parser)
{
	parser->buffer[0] = '\0';
	parser->buffer_pos= 0;
}
