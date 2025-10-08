/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:26:02 by ludovico          #+#    #+#             */
/*   Updated: 2025/10/08 17:00:52 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int fork_and_execute(t_cmd *cmd_left, int *status, t_shell_state *shell, int *pipefd)
{
	pid_t	left_pid;
	pid_t	right_pid;
	t_cmd	*cmd_right;

	cmd_right = cmd_left->next;
	left_pid = fork();
	if (left_pid == -1)
	{
		setup_signals(INTERACTIVE);
		return (fork_error(pipefd, NULL, NULL, 0));
	}
	if (left_pid == 0)
		exit(exec_pipeline_left(cmd_left, shell, pipefd));
	right_pid = fork();
	if (right_pid == -1)
	{
		setup_signals(INTERACTIVE);
		return (fork_error(pipefd, &left_pid, NULL, 0));
	}
	if (right_pid == 0)
		exit(exec_pipeline_right(cmd_right, shell, pipefd));
	fork_close(pipefd, &left_pid, &right_pid, status);
	return (0);
}

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

