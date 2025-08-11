/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-24 14:28:03 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	char	*str;
	char	*dest;
	size_t	len;

	if (!s1)
		return (ft_strjoin(s2, s3));
	if (!s2)
		return (ft_strjoin(s1, s3));
	if (!s3)
		return (ft_strjoin(s1, s2));
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	str = (char *)ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	dest = str;
	while (*s1)
		*dest++ = *s1++;
	while (*s2)
		*dest++ = *s2++;
	while (*s3)
		*dest++ = *s3++;
	return (str);
}
