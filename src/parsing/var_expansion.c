/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:29:51 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/20 14:14:57 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	expand_variables(t_token_list *token_list)
{
	t_token	*curr_token;
	
	if (!token_list)
		return (-1);
	curr_token = token_list->head;
	while (curr_token)
	{
		if (curr_token->type == VARIABLE)
			expand_var_token(curr_token);
		// else if (curr_token->type == QUOTED_STRING)
		// {
		// 	//cerca var
		// 	//espandi
		// }
		curr_token = curr_token->next;
	}
	return (0);
}

int	expand_var_token(t_token *token)
{
	char	*expanded_val;

	expanded_val = getenv(token->content);
	free(token->content);
	if (expanded_val)
		token->content = ft_strdup(expanded_val);
	else
		token->content = ft_strdup("");
	token->type = WORD;
	return (0);
}
