/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:50:34 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/23 18:32:34 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Global head pointer for the garbage collector
t_grb_node	*init_grb_collector(void)
{
	t_grb_node	*head;

	head = malloc(sizeof(t_grb_node));
	if (!head)
		return (NULL);
	head->ptr = NULL;
	head->next = NULL;
	return (head);
}

// Register a pointer for garbage collection
void	*malloc_grb(size_t size, t_grb_node *head)
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
		if (current->ptr != NULL)
			free (current->ptr);
		free(current);
		current = temp;
	}
	return ;
}
