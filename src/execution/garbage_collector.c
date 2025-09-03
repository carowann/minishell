/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:50:34 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/03 16:30:34 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Global head pointer for the garbage collector
t_grb_node *init_grb_collector(void)
{
	t_grb_node *head;
	
	head = malloc(sizeof(t_grb_node));
	if (!head)
		return (NULL);
	head->ptr = NULL;
	head->next = NULL;
	return (head);
}

// Register a pointer for garbage collection
void *malloc_grb(size_t size, t_grb_node *head)
{
	void		*mem;
	t_grb_node	*node;

	mem = malloc(size);
	if (!mem)
		return (NULL);
	node = malloc(sizeof(t_grb_node));
	if (!node)
	{
		free(mem);
		return (NULL);
	}
	node->ptr = mem;
	node->next = head;
	head = node;
	return (mem);
}

// Free all registered pointers
void	take_out_grb(t_grb_node *head)
{
	t_grb_node	*current;
	t_grb_node	*temp;

	if (!head)
		return ;
	current = head;
	while (current)
	{
		temp = current->next;
		if(current->ptr != NULL)
			free (current->ptr);
		free(current);
		current = temp;
	}
	return;
}
