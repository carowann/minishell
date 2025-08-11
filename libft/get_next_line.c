/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 18:20:48 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*handle_read_return(char **stash, char *buffer, int n_bytes)
{
	char	*line_read;

	line_read = NULL;
	free(buffer);
	if (n_bytes == 0)
	{
		if (*stash && **stash)
		{
			line_read = ft_strndup(*stash, ft_strlen(*stash));
			free(*stash);
			*stash = NULL;
			return (line_read);
		}
	}
	free (*stash);
	*stash = NULL;
	return (NULL);
}

static char	*extract_line_from_stash(char **stash)
{
	size_t	line_len;
	char	*line_read;
	char	*temp;

	line_read = NULL;
	line_len = ft_strchr(*stash, '\n') - *stash + 1;
	line_read = ft_strndup(*stash, line_len);
	if (ft_strlen(*stash) > line_len)
	{
		temp = ft_strndup(&((*stash)[line_len]), ft_strlen(*stash) - line_len);
		free(*stash);
		*stash = temp;
		return (line_read);
	}
	free(*stash);
	*stash = NULL;
	return (line_read);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*temp;
	int			n_bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (!stash || !ft_strchr(stash, '\n'))
	{
		buffer = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!buffer)
			return (NULL);
		n_bytes = read(fd, buffer, BUFFER_SIZE);
		if (n_bytes <= 0)
			return (handle_read_return(&stash, buffer, n_bytes));
		temp = ft_strjoin(stash, buffer);
		free(stash);
		stash = temp;
		free(buffer);
	}
	return (extract_line_from_stash(&stash));
}
