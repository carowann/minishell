/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 12:29:51 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/28 10:36:58 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: gestire $?

/*
 * Expands variables if token type is variable or quoted string
 * @param token_list: list of tokens to possibly expand
 * @return: 0 success, -1 error
 */
int	expand_variables(t_env *env, t_token_list *token_list) 
{
	t_token	*curr_token;

	if (!token_list)
		return (-1);
	curr_token = token_list->head;
	while (curr_token)
	{
		if (curr_token->type == VARIABLE)
		{
			if (expand_single_var(env, curr_token) == -1)
				return (-1);
		}
		else if (curr_token->type == QUOTED_STRING)
		{
			if (handle_var_in_str(env, curr_token) == -1)
				return (-1);
		}
		curr_token = curr_token->next;
	}
	return (0);
}

/*
 * Expands variable in a token of type variable
 * @param token: token to expand
 * @return: 0 success, -1 error
 */
int expand_single_var(t_env *env, t_token *token)
{
	char *var_value;

	var_value = get_env_value(env, token->content);
	free(token->content);
	token->content = ft_strdup(var_value);
	if (!token->content)
		return (-1);
	token->type = WORD;
	return (0);
}

/*
 * Expansion of variable in string of token of type quoted string
 * @param token: token with string with variable to expand
 * @return: 0 success, -1 error
 */
int handle_var_in_str(t_env *env, t_token *token)
{
	char *expanded_content;

	expanded_content = process_string_expansion(env, token->content);
	if (!expanded_content)
		return (-1);
	free(token->content);
	token->content = expanded_content;
	token->type = WORD;
	return (0);
}

/*
 * Looks for var to expand in string, expands it and puts value back in string
 * @param str: string with variable to expand
 * @return: finalized string with expansion
 */
char *process_string_expansion(t_env *env, char *str)
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
			result = expand_var_in_str(env, str, &i, result);
		else
			result = append_char(result, str[i++]);
		if (!result)
			return (NULL);
	}
	return (result);
}

/*
 * Extracts, var name from string, expands it and joins with old string
 * @param str: string with variable to expand
 * @param i: index of dollar sign in string
 * @param old_str: old string to update w/ expansion of variable
 * @return: finalized string with expansion
 */
char	*expand_var_in_str(t_env *env, char *str, int *i, char *old_str)
{
	int		var_len;
	char	*var_name;
	char	*var_value;
	char	*new_str;

	var_len = 0;
	var_name = extract_var_name(str, i, &var_len);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(env, var_name);
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

