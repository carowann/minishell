/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:57:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/21 18:10:39 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_env_value(char *var_name)
{
	char *value;

	value = getenv(var_name);
	if (value)
		return (value);
	return ("");
}

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
