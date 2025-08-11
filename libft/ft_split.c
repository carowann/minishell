/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:12:23 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	count_strings(char const *s, char c)
{
	size_t			count;
	unsigned char	in_string;

	count = 0;
	in_string = 0;
	while (*s)
	{
		if (*s != c && !in_string)
		{
			in_string = 1;
			count++;
		}
		else if (*s == c)
			in_string = 0;
		s++;
	}
	return (count);
}

static	void	*free_array(char **array, size_t i)
{
	while (i > 0)
	{
		free(array[i - 1]);
		i--;
	}
	free(array);
	return (NULL);
}

static char	*extract_token(char **s, char c)
{
	char	*start;
	size_t	len;
	char	*token;

	while (**s == c)
		(*s)++;
	start = *s;
	while (**s && **s != c)
		(*s)++;
	len = *s - start;
	token = ft_calloc(len + 1, sizeof(char));
	if (token)
		ft_strlcpy(token, start, len + 1);
	return (token);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	char	*str;
	size_t	n_strings;
	size_t	i;

	n_strings = count_strings(s, c);
	if (n_strings == 0)
		return (NULL);
	array = (char **)ft_calloc(n_strings + 1, sizeof(char *));
	if (!array)
		return (NULL);
	str = (char *)s;
	i = 0;
	while (i < n_strings)
	{
		array[i] = extract_token(&str, c);
		if (!array[i])
			return (free_array(array, i));
		i++;
	}
	return (array);
}
