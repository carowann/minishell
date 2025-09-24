#include "../includes/minishell.h"

int set_last_exit_status(t_shell_state *shell, int status)
{
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
	else
		shell->last_exit_status = 1;
	return (shell->last_exit_status);
}

int pipe_error(int *fd)
{
    if (pipe(fd) == -1)
	{
		perror("pipe in heredoc failed");
		return (1);
	}
    return (0);
}

int	fork_close(int *fd, pid_t *whait1, pid_t *whait2, int *status)
{
	close(fd[0]);
	close(fd[1]);
	if (whait1)
		waitpid(*whait1, NULL, 0);
	if (whait2)
		waitpid(*whait2, status, 0);
	return (-1);
}

int fork_error(int *fd, pid_t *whait1, pid_t *whait2, int *status)
{
	perror("fork failed");
	fork_close(fd, whait1, whait2, status);
	return (-1);
}
