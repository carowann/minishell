/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expansion_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:57:12 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/03 18:27:28 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Expands variable end returns value
 * @param var_name: name of variable to expand
 * @return: value of expanded variable
 */
char *get_env_value(t_shell_state *shell, char *var_name)
{
	char *value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		value = ft_itoa(shell->last_exit_status);
	else
		value = extract_value_from_env_list(shell->env_list, var_name);
	if (value)
		return (value);
	return (ft_strdup(""));
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

/*
 * Extracts value of variable from env list
 * @param env: env list to extract from
 * @param var_name: target variable name
 * @return: variable value
 */
char	*extract_value_from_env_list(t_env *env, char *var_name)
{
	t_env	*env_node;
	char	*var_value;

	env_node = find_env(env, var_name);
	if (!env_node)
		return (NULL);
	var_value = get_value_from_env_str(env_node->value);
	if (!var_value)
		return (NULL);
	return (var_value);
}

/*
 * Extracts value from string of env list w/ format var=vale
 * @param env_str: string to analyse
 * @return: variable value
 */
char	*get_value_from_env_str(char *env_str)
{
	char	*var_value;
	char	*start_var_value;

	start_var_value = ft_strchr(env_str, '=');
	if (!start_var_value)
		return (NULL);
	start_var_value = start_var_value + 1;
	var_value = ft_strdup(start_var_value);
	if (!var_value)
		return (NULL);
	return (var_value);
}
