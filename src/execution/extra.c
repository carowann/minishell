#include "../../includes/minishell.h"

char *get_all_line(int fd)
{
	char *str;
	char *line;

	str = ft_strdup("");
	line = get_next_line(fd);
	if (!line)
	{
		free(str);
		return (NULL);
	}
	str = ft_strjoin(str, line);
	free(line);
	while ((line = get_next_line(fd)))
	{
		str = ft_strjoin(str, line);
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

pipe_error(int *fd)
{
    if (pipe(fd) == -1)
	{
		perror("pipe in heredoc failed");
		return (1);
	}
    return (0);
}