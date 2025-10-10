/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:41:44 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/10 15:42:28 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_all_line(int fd)
{
	char	*str;
	char	*line;
	char	*temp;

	str = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		temp = str;
		str = ft_strjoin(str, line);
		free(temp);
		free(line);
	}
	return (str);
}

void	free_command_all(t_cmd *cmd)
{
	t_cmd	*temp_cmd;

	if (!cmd)
		return ;
	while (cmd)
	{
		temp_cmd = cmd->next;
		free_cmd(cmd);
		cmd = temp_cmd;
	}
}

int	heredoc_execve(t_cmd *cmd)
{
	int	fd[2];

	pipe(fd);
	if (fd[0] < 0 || fd[1] < 0)
	{
		perror("pipe in heredoc failed");
		return (-1);
	}
	if (cmd->heredoc_delimiter)
		write(fd[1], cmd->heredoc_delimiter, ft_strlen(cmd->heredoc_delimiter));
	else
		write(fd[1], "", 1);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	return (0);
}

int	heredoc_status(int *fd, t_shell_state *shell)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	shell->last_exit_status = 130;
	return (-1);
}

int	heredoc_closing(t_cmd *cmd, int *fd)
{
	char	*line;

	close(fd[1]);
	line = get_all_line(fd[0]);
	close(fd[0]);
	if (!line)
	{
		free(cmd->heredoc_delimiter);
		cmd->heredoc_delimiter = NULL;
	}
	else
	{
		free(cmd->heredoc_delimiter);
		cmd->heredoc_delimiter = ft_strdup(line);
	}
	cmd->is_heredoc = 2;
	free(line);
	return (0);
}
