/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:29:51 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/21 17:57:22 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: gestire $?

int	expand_variables(t_token_list *token_list) 
{
	t_token	*curr_token;

	if (!token_list)
		return (-1);
	curr_token = token_list->head;
	while (curr_token)
	{
		if (curr_token->type == VARIABLE)
		{
			if (expand_single_var(curr_token) == -1)
				return (-1);
		}
		else if (curr_token->type == QUOTED_STRING)
		{
			if (handle_var_in_str(curr_token) == -1)
				return (-1);
		}
		curr_token = curr_token->next;
	}
	return (0);
}

int expand_single_var(t_token *token)
{
	char *var_value;

	var_value = get_env_value(token->content);
	free(token->content);
	token->content = ft_strdup(var_value);
	if (!token->content)
		return (-1);
	token->type = WORD;
	return (0);
}

int handle_var_in_str(t_token *token)
{
	char *expanded_content;

	expanded_content = process_string_expansion(token->content);
	if (!expanded_content)
		return (-1);
	free(token->content);
	token->content = expanded_content;
	token->type = WORD;
	return (0);
}

char *process_string_expansion(char *str)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
			result = expand_var_in_str(str, &i, result);
		else
			result = append_char(result, str[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*expand_var_in_str(char *str, int *i, char *old_str)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	char	*new_str;

	var_len = 0;
	var_name = extract_var_name(str, i, &var_len);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(var_name);
	free(var_name);
	new_str = ft_strjoin(old_str, var_value);
	if (!new_str)
	{
		free(old_str);
		return (NULL);
	}
	free(old_str);
	return (new_str);
}

