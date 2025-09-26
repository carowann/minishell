/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:29:56 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/25 18:33:23 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Check if the command is valid inbuilt command
int	is_valid_cmd(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
    if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
    if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
    if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int command_select(t_cmd *cmd, t_shell_state **shell)
{
	int	fd[1];
	int	result;

	result = 0;
	if (open_in(cmd, fd) == -1)
		return (1);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		result = echo_builtin(cmd->args, fd[0]);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		result = cd_builtin(cmd, shell);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		result = pwd(fd[0]);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		result = export(cmd, shell, fd[0]);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		result = unset(cmd, shell);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		result = env((*shell)->env_list, fd[0], 0);
	else if (ft_strncmp(cmd->args[0], "exit", 4) == SUCCESS)
		return (builtin_exit(cmd, *shell));
	else if (ft_printfd(2, "minishell: %s: command not found\n", cmd->args[0]))
		result = 127;
	if (fd[0] != STDOUT_FILENO)
		close(fd[0]);
	return (result);
}

int open_in(t_cmd *cmd, int *fd)
{
	int i;

	i = 0;
	while (cmd->output_files[i + 1])
	{
		ft_printfd(1, "Opening output file: %s\n", cmd->output_files[i]);
		fd[0] = open(cmd->output_files[i], O_RDWR | O_CREAT | (cmd->append_mode * O_APPEND)
			| (!cmd->append_mode * O_TRUNC),  OUTFILE_PERMS);
		if (fd[0] < 0)
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_files[i]);
		if (fd[0] < 0)
			return (-1);
		close(fd[0]);
		i++;
	}
	fd[0] = open(cmd->output_file, O_RDWR | O_CREAT | (cmd->append_mode * O_APPEND)
			| (!cmd->append_mode * O_TRUNC),  OUTFILE_PERMS);
	if (fd[0] < 0)
	{
		ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
		return (-1);
	}
	return (1);
}
