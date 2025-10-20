/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/10/20 19:35:32 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	set_exit_status(t_shell_state **shell, int status)
{
	(*shell)->last_exit_status = status;
	return (status);
}

static int	handle_pipeline(t_cmd *cmd, t_shell_state **shell)
{
	int	result;

	result = pipeman(cmd, cmd->next, *shell);
	(*shell)->last_exit_status = result;
	return (result);
}

// Function to execute a command based on its type
int	execute_cmd(t_cmd *cmd, t_shell_state **shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (set_exit_status(shell, 1));
	if (cmd->next != NULL)
		return (handle_pipeline(cmd, shell));
	if (cmd->is_heredoc == 1)
	{
		setup_signals(PIPELINE);
		if (set_up_heredoc(cmd, *shell) == -1)
		{
			setup_signals(INTERACTIVE);
			return ((*shell)->last_exit_status);
		}
		setup_signals(INTERACTIVE);
	}
	if (is_valid_cmd(cmd->args[0]))
		return (handle_builtin(cmd, shell));
	return (handle_external_command(cmd, shell));
}

int	handle_builtin(t_cmd *cmd, t_shell_state **shell)
{
	int	result;

	result = command_select(cmd, shell);
	if (ft_strncmp(cmd->args[0], "exit", 5) != 0 || !(*shell)->should_exit)
		(*shell)->last_exit_status = result;
	return (result);
}

int	handle_external_command(t_cmd *cmd, t_shell_state **shell)
{
	char	*exe_path;
	int		fd[2];
	int		result;

	exe_path = build_exe_path(*shell, cmd);
	if (!exe_path)
	{
		fd[1] = -1;
		open_ve_out(fd, cmd);
		if (fd[1] != -1)
			close(fd[1]);
		ft_printfd(STDERR_FILENO, "minishell: %s:", cmd->args[0]);
		ft_printfd(STDERR_FILENO, " command not found\n");
		return (set_exit_status(shell, 127));
	}
	setup_signals(EXECUTION);
	result = execve_temp(exe_path, cmd, shell);
	setup_signals(INTERACTIVE);
	free(exe_path);
	return (set_exit_status(shell, result));
}
