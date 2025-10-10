/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:24:54 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/10 18:50:46 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function to read a line from standard input
// char *read_line(void) //TODO: change forbidden functions!!!!
// {
// 	char buffer[1024];
// 	if (fgets(buffer, sizeof(buffer), stdin) == NULL)
// 		return NULL;
// 	size_t len = strlen(buffer);
// 	if (len > 0 && buffer[len - 1] == '\n')
// 		buffer[len - 1] = '\0'; // Remove newline
// 	return strdup(buffer);		// Allocate and return the string
// }

char **env_to_matrix(t_env *env)
{
	char	**matrix;
	t_env	*copy;
	int		i;

	i = 0;
	copy = env;
	while (copy)
	{
		i++;
		copy = copy->next;
	}
	matrix = malloc(sizeof(char *) * (i + 1));
	if (!matrix)
		return (NULL);
	matrix[i] = NULL;
	copy = env;
	i = 0;
	while (copy)
	{
		matrix[i] = ft_strdup(copy->value);
		i++;
		copy = copy->next;
	}
	return (matrix);
}

char **dup_matrix(char **matrix)
{
	char	**dup;
	int		i;

	i = 0;
	while (matrix[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	if (!dup)
		return (NULL);
	dup[i] = NULL;
	i = 0;
	while (matrix[i])
	{
		dup[i] = ft_strdup(matrix[i]);
		i++;
	}
	return (dup);
}

int open_placebo_all(t_cmd *cmd, int *fd)
{
	int i;

	i = 0;
	if (cmd->input_file != NULL)
	{
		while (i < cmd->input_count)
		{
			if (open_in_placebo(cmd, fd, i, 0) == -1)
				return (-1);
			i++;
		}
	}
	return (0);
}