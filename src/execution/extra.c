#include "../../includes/minishell.h"

char *get_all_line(int fd)
{
	char *str;
	char *line;
	char *temp;

	str = NULL;
	line = get_next_line(fd);
	if (!line)
	{
		free(str);
		return (NULL);
	}
	temp = str;
	str = ft_strjoin(str, line);
	free(temp);
	free(line);
	while ((line = get_next_line(fd)))
	{
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

	if (!cmd || (!cmd->next))
		return ;
	while (cmd && cmd->next)
	{	
		temp_cmd = cmd->next;
		free_cmd(cmd);
		cmd = temp_cmd;
	}
	free_cmd(cmd);
	return ;
}

int heredoc_execve(t_cmd *cmd)
{
	int fd[2];
	pipe(fd);
	if (fd[0] < 0 || fd[1] < 0)
	{
		perror("pipe in heredoc failed");
		return (-1);
	}
	write(fd[1], cmd->input_file, ft_strlen(cmd->input_file));
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	return (0);
}
