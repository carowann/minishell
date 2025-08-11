/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 17:59:11 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_node;
	t_list	*new_lst;
	t_list	*last_node;
	void	*new_content;

	if (!lst || !f)
		return (NULL);
	new_lst = NULL;
	last_node = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		if (!new_content)
			return (ft_lstclear(&new_lst, del), NULL);
		new_node = ft_lstnew(new_content);
		if (!new_node)
			return (del(new_content), ft_lstclear(&new_lst, del), NULL);
		if (!new_lst)
			new_lst = new_node;
		else
			last_node->next = new_node;
		last_node = new_node;
		lst = lst->next;
	}
	return (new_lst);
}
