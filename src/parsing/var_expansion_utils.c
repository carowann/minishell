/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:57:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/21 18:41:50 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * Expands variable end returns value
 * @param var_name: name of variable to expand
 * @return: value of expanded variable
 */
char *get_env_value(char *var_name)
{
	char *value;

	value = getenv(var_name);
	if (value)
		return (value);
	return ("");
}

/*
 * Extracts variable name from string
 * @param str: string source of variable name
 * @param dollar_pos: index fo dollar position in string
 * @param var_len: length ov variable name
 * @return: name of variable to expand
 */
char	*extract_var_name(char *str, int *dollar_pos, int *var_len)
{
	int	start;
	int	end;

	start = *dollar_pos + 1;
	end = start;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	*var_len = end - start;
	if (!*var_len)
		return (NULL);
	*dollar_pos = *dollar_pos + *var_len + 1;
	return (ft_substr(str, start, *var_len));
}

/*
 * Appends char to string
 * @param str: string
 * @param c: character to append to string
 * @return: finalized string
 */
char	*append_char(char *old_str, char c)
{
	char	to_app[2];
	char	*new_str;

	to_app[0] = c;
	to_app[1] = '\0';
	new_str = ft_strjoin(old_str, to_app);
	if (!new_str)
	{
		free(old_str);
		return (NULL);
	}
	free(old_str);
	return (new_str);
}
