#include "../../includes/minishell.h"

int handle_heredoc(t_cmd *cmd, t_shell_state **shell)
{
	int   pipefd[2];
	pid_t pid;
	int   status;

	if (pipe_error(pipefd) == 1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (fork_error(pipefd, NULL, NULL, NULL));
	if (pid == 0) //child uno, legge da stdin e scrive su pipefd[1]
		exit(doc_child_write(cmd, pipefd, shell));
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		pid = fork();
		if (pid == -1)
			return (fork_error(pipefd, NULL, NULL, NULL));
		if (pid == 0) //child due, legge da pipefd[0] e duplica su stdin
			exit(doc_child_read(cmd, pipefd, shell));
		close(pipefd[0]);
		waitpid(pid, &status, 0);
		return set_last_exit_status(*shell, status);
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

int doc_child_write(t_cmd *cmd, int *fd, t_shell_state **shell)
{
	close(fd[0]);
	heredoc_read(fd, cmd->heredoc_delimiter);
	close(fd[1]);
	free_command_all((*shell)->current_cmd_list->head);
	free((*shell)->current_cmd_list);
	free_env((*shell)->env_list);
	free(*shell);
	return(0);
}

int doc_child_read(t_cmd *cmd, int *fd, t_shell_state **shell)
{
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	if (is_valid_cmd(cmd->args[0]))
		handle_builtin(cmd, shell);
	else 
		handle_external_command(cmd, shell);
	pipe_free_all((*shell)->current_cmd_list->head, *shell);
	return((*shell)->last_exit_status);
}
