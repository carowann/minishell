/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/08/20 18:32:02 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include "templib.h"


// Function to execute a command based on its type	
int execute_cmd(t_cmd *cmd, t_env *envar)
{
	int fd;// File descriptor for input redirection

	fd = 1;
    if (!cmd || !cmd->args || !cmd->args[0])
        return (-1);
	if (cmd->input_file)
	{
		int fd = open(cmd->input_file, O_RDONLY);
		if (fd < 0)
		{
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->input_file);
			return (-1);	
		}
	}
    if (is_valid_cmd(cmd->args[1]))
        command_select(cmd, fd, envar);
    else
    {
        ft_printfd(1, "minishell: %s: command not found\n", cmd->args[0]);
		if (fd > 0)
			close(fd);
		return (-1);
    }
	if (fd > 1)
		close(fd);
	return (1);
}
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
// Function to select the command execution based on the command type
// This function will call the appropriate execution function based on the command type
// It returns 1 on success, -1 on failure
int command_select(t_cmd *cmd, int fd, t_env *envar)
{
	if (ft_strncmp(cmd->args[1], "echo", 5) == 0)
		return (echo(cmd->args, fd, 1));
	// else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
	// 	return (cd_exec(cmd->args));
	else if (ft_strncmp(cmd->args[1], "pwd", 4) == 0)
		return (pwd(fd));
	// else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
	// 	return (export_exec(cmd->args));
	// else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
	// 	return (unset_exec(cmd->args));
	else if (ft_strncmp(cmd->args[1], "env", 4) == 0)
		return (env(fd, envar));
	// else    if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	// 	return (exit_exec(cmd->args));
	else
	{
		ft_printfd(1, "minishell: %s: command not found\n", cmd->args[0]);
		return (-1);
	}
}
