/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:29:51 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/20 17:45:03 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	expand_variables(t_token_list *token_list) //TODO: gestire $?
{
	t_token	*curr_token;

	if (!token_list)
		return (-1);
	curr_token = token_list->head;
	while (curr_token)
	{
		if (curr_token->type == VARIABLE)
			expand_var_token(curr_token);
		else if (curr_token->type == QUOTED_STRING)
			expand_var_in_string(curr_token, curr_token->content);
		curr_token = curr_token->next;
	}
	return (0);
}

void	expand_var_token(t_token *token)
{
	char	*expanded_val;

	expanded_val = getenv(token->content);
	free(token->content);
	if (expanded_val)
		token->content = ft_strdup(expanded_val);
	else
		token->content = ft_strdup("");
	token->type = WORD;
}

int	expand_var_in_string(t_token *token, char *str)
{
	int		i;
	int		start;
	int		end;
	char	*var_name;
	char	*res_str;

	if (!token)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			start = i + 1;
			end = start;
			while (str[end] && (ft_isalnum(str[end] || str[end] == '_')))
				end++;
			var_name = ft_substr(str, start, end - start);
			if (!var_name)
				return (-1);
			if (!)
			i = end;
		}
		else
		//concatenare a stringa
		i++;
	}
	return (0);
}

