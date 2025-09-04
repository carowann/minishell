/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/04 18:53:58 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: fix leak w/ redirection w/ non existent file
//minishell> cat < no.t

static int set_exit_status(t_shell_state **shell, int status);
static int handle_pipeline(t_cmd *cmd, t_shell_state **shell);
static int handle_builtin(t_cmd *cmd, t_shell_state **shell);
static int handle_external_command(t_cmd *cmd, t_shell_state **shell);

// Function to execute a command based on its type
int execute_cmd(t_cmd *cmd, t_shell_state **shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (set_exit_status(shell, 1));
	if (cmd->next != NULL)
		return (handle_pipeline(cmd, shell));
	if (is_valid_cmd(cmd->args[0]))
		return (handle_builtin(cmd, shell));
	return (handle_external_command(cmd, shell));
}

static int set_exit_status(t_shell_state **shell, int status)
{
	(*shell)->last_exit_status = status;
	return (status);
}

static int handle_pipeline(t_cmd *cmd, t_shell_state **shell)
{
	int result;

	result = pipeman(cmd, cmd->next, *shell);
	(*shell)->last_exit_status = result;
	return (result);
}

static int handle_builtin(t_cmd *cmd, t_shell_state **shell)
{
	int result;

	result = command_select(cmd, shell);
	if (ft_strncmp(cmd->args[0], "exit", 5) != 0)
		(*shell)->last_exit_status = result;
	return (result);
}

static int handle_external_command(t_cmd *cmd, t_shell_state **shell)
{
	char *exe_path;
	int result;

	exe_path = build_exe_path(*shell, cmd);
	if (!exe_path)
	{
		ft_printfd(STDERR_FILENO, "minishell: %s: command not found\n", cmd->args[0]);
		return (set_exit_status(shell, 127));
	}
	printf("DEBUG1: Before execve - exe_path=%p (%s), cmd->args[0]=%s\n", 
		exe_path, exe_path, cmd->args[0]);
	result = execve_temp(exe_path, cmd, (*shell)->env_list);
	printf("DEBUG1: After execve - exe_path=%p (%s), cmd->args[0]=%s, result=%d\n", 
		exe_path, exe_path, cmd->args[0], result);
	free(exe_path);
	printf("DEBUG1: After free - cmd->args[0]=%s\n", cmd->args[0]);
	return (set_exit_status(shell, result));
}
