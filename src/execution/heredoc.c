#include "../../includes/minishell.h"

int handle_heredoc(const char *delimiter, t_cmd *cmd, t_shell_state **shell)
{
	int   pipefd[2];
	pid_t pid;
	int   status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_read(pipefd, delimiter);
		close(pipefd[1]);
		free_command_all((*shell)->current_cmd_list->head);
		free((*shell)->current_cmd_list);
		free_env((*shell)->env_list);
		free(*shell);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			close(pipefd[0]);
			return (1);
		}
		if (pid == 0)
		{
			dup2(pipefd[0], STDIN_FILENO);
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
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				(*shell)->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				(*shell)->last_exit_status = 128 + WTERMSIG(status);
			return ((*shell)->last_exit_status);
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

void heredoc_sub(t_cmd *cmd, int *fd)
{
	close(fd[0]);
	heredoc_read(fd, cmd->heredoc_delimiter);
	close(fd[1]);
	free_command_all(cmd);
	return ;
}