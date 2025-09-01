/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:19:41 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/01 15:04:50 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function to convert the envp to a linked list
t_env	*env_to_list(char **envp)
{
	t_env	*head;
	int		i;  
	t_env	*current;

	head = NULL;
	i = 0;
	while (envp[i])
	{
	current = malloc(sizeof(t_env));
		if (!current)
			return (NULL);
		current->value = ft_strdup(envp[i]);
		if (!current->value)
		{
			free(current);
			return (NULL);
		}
		current->next = head;
		head = current;
		i++;
	}
	return (head);
}
void	free_env(t_env *head)
{
	t_env	*current;
	t_env	*temp;

	
	if (!head)
		return ;
	current = head;
	while (current)
	{
		temp = current->next;
		if(current->value)
			free (current->value);
		free(current);
		current = temp;
	}

	return;
}