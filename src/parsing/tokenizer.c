/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 16:31:58 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/12 19:08:27 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//token list: struttura temportanea per i token
//command list: struttura finale per execution
//dopo il parsing, token list viene distrutta e command list viene usata per l'esecuzione

t_token_list *tokenize(char *input)
{
	t_token_list	*tokens;
	t_state			state;
	char			*buffer;
	
	tokens = ft_calloc(1, sizeof(t_token_list));
	if (!tokens)
		handle_error();
	buffer = ft_calloc(1, sizeof(ft_strlen(input) + 1));
	if (!buffer)
		handle_error();
	*tokens = (t_token_list){0};
	state = DEFAULT;
	while (*input)
	{
		handle_state_machine(*input, state, buffer, tokens);
		//a seconda dello stato in cui mi trovo creo o meno il token (automa a stati finit)
		//se ho creato il token lo aggiungo alla fine della lista
		input++;
	}
	free(buffer);
	return (tokens);
}


