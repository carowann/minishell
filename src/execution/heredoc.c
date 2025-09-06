#include "../../includes/minishell.h"

int handle_heredoc(const char *delimiter, t_cmd *cmd, t_shell_state **shell)
{
	char *line;
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
