#include "../../includes/minishell.h"

int handle_heredoc(const char *delimiter, t_cmd *cmd, t_shell_state **shell)
{
	int   pipefd[2];
	pid_t pid_write;
	pid_t pid_read;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid_read = fork();
	if (pid_read == 0)
	{
		close(pipefd[0]);
		heredoc_read(pipefd, delimiter);
		free_command_all((*shell)->current_cmd_list->head);
		free((*shell)->current_cmd_list);
		free_env((*shell)->env_list);
		free(*shell);
		exit(0);
	}
	else // Parent process
	{
		close(pipefd[1]);
		waitpid(pid_read, NULL, 0);
		pid_write = fork();
		if (pid_write == 0)
		{
			dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read
			close(pipefd[0]);
			if (is_valid_cmd(cmd->args[0]))
				handle_builtin(cmd, shell);
			else 
				handle_external_command(cmd, shell);
			free_command_all((*shell)->current_cmd_list->head);
			free((*shell)->current_cmd_list);
			free_env((*shell)->env_list);
			free(*shell);
			exit((*shell)->last_exit_status);
		}
		else  // Parent process
		{
			close(pipefd[0]);
			waitpid(pid_write, NULL, 0);
			return (0);
		}
	}
}

int heredoc_read(int *pipefd, const char *delimiter)
{
	char *line;

	while (1)
	{
		ft_printfd(1, "> ");
		line = read_line();
		if (!line || strcmp(line, delimiter) == 0)
			break;
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipefd[1]);
	return (0);
}

// char **heredoc_pipe(t_cmd *cmd)
// {
// 	char **ret;
// 	char *line;
// 	t_cmd *heredoc_cmd;
// 	int   i;

// 	i = 0;
// 	heredoc_cmd = cmd;
// 	ret = malloc(sizeof(char *) * (cmd->arg_count + 1));
// 	while (heredoc_cmd != NULL)
// 	{
// 		while(!heredoc_cmd->is_heredoc && heredoc_cmd != NULL)
// 		{
// 			heredoc_cmd = heredoc_cmd->next;
// 		}
// 		while (heredoc_cmd != NULL && heredoc_cmd->is_heredoc)
// 		{
// 			ft_printfd(1, "> ");
// 			line = read_line();
// 			if (!line || strcmp(line, heredoc_cmd->heredoc_delimiter) == 0)
// 				break;
// 			line = ft_strjoin(line, "\n");
// 			ret[i] = ft_strjoin(ret[i], line);
// 			free(line);
// 		}
// 		if (heredoc_cmd)
// 			heredoc_cmd = heredoc_cmd->next;
// 		i++;
// 	}
// 	return (ret);
// }
