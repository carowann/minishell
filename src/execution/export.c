/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:49:20 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/02 13:49:47 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: sort env in alphabetical order

int export(t_cmd *cmd, t_shell_state **shell, int fd)
{
	int i = 1;

	if (!cmd->args[1])
		return (env((*shell)->env_list, fd, 1));

	while (cmd->args[i])
	{
		if (ft_isalpha(cmd->args[i][0]) == 0 && cmd->args[i][0] != '_')
		{
			ft_printf("minishell: export: `%s': not a valid identifier\n", cmd->args[i]);
			return (-1);
		}

		if (!find_env((*shell)->env_list, cmd->args[i]))
			add_env(&((*shell)->env_list), cmd->args[i]);
		else
			update_env((*shell)->env_list, cmd->args[i]);
		i++;
	}
	return (1);
}

// Function to add a new environment variable to the list
int add_env(t_env **envar, char *arg)
{
	t_env *new;
	t_env *temp;

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
int update_env(t_env *envar, char *arg)
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
