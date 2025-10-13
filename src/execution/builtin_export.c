/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:49:20 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/13 15:42:54 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	export(t_cmd *cmd, t_shell_state **shell, int fd)
{
	int	i;
	int	has_error;

	has_error = 0;
	i = 1;
	if (!cmd->args[1])
		return (env((*shell)->env_list, fd, 1));
	while (cmd->args[i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			ft_printfd(2, "minishell: export: ");
			ft_printfd(2, "`%s': invalid id\n", cmd->args[i]);
			has_error = 1;
		}
		else
		{
			if (!find_env((*shell)->env_list, cmd->args[i]))
				add_env(&((*shell)->env_list), cmd->args[i]);
			else
				update_env((*shell)->env_list, cmd->args[i]);
		}
		i++;
	}
	return (has_error);
}

// Function to add a new environment variable to the list
int	add_env(t_env **envar, char *arg)
{
	t_env	*new;
	t_env	*temp;

	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	new->value = ft_strdup(arg);
	if (!new->value)
	{
		free(new);
		return (-1);
	}
	new->next = NULL;
	if (!*envar)
	{
		*envar = new;
		return (1);
	}
	temp = *envar;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (1);
}

// Function to update the value of an existing environment variable
int	update_env(t_env *envar, char *arg)
{
	t_env	*temp;
	char	*new_value;

	temp = find_env(envar, arg);
	if (!temp)
		return (-1);
	new_value = ft_strdup(arg);
	if (!new_value)
		return (-1);
	free(temp->value);
	temp->value = new_value;
	return (1);
}
