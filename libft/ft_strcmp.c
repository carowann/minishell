/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:06:29 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/03 14:07:12 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * Compares two null-terminated strings
 * @param s1: first string to compare
 * @param s2: second string to compare
 * @return: 0 if strings are equal
 *          < 0 if s1 < s2
 *          > 0 if s1 > s2
 */
int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
	{
		if (s1 == s2)
			return (0);
		if (!s1)
			return (-1);
		return (1);
	}
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}