/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 16:15:13 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/26 16:15:52 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int execve_matr_fail(char **envp, char **temp, t_cmd *cmd, t_shell_state **shell)
{
	if (envp)
		free_matrix(envp);
	if (temp)
		free_matrix(temp);
	(void)cmd;
	free_command_all((*shell)->current_cmd_list->head);
	free((*shell)->current_cmd_list);
	free_env((*shell)->env_list);
    return (EXIT_FAILURE);
}
//TODO: is_valid_exe_path dovrebbe bastare, dimmi se ci sono problemi
char	*find_cmd_exe(char **paths, t_cmd *cmd)
{
	size_t	i;
	char	*exe_path;

	i = 0;
	if (!cmd || !cmd->args[0] || !cmd->args[0][0])
		return (NULL);
	if (ft_strchr(cmd->args[0], '/') && is_valid_exe_path(cmd->args[0]) == SUCCESS)
		return (ft_strdup(cmd->args[0]));
	while (paths[i])
	{
		exe_path = ft_strjoin3(paths[i], "/", cmd->args[0]);
		if (!exe_path)
			return (NULL);
		if (is_valid_exe_path(exe_path) == SUCCESS)
			return (exe_path);
		free(exe_path);
		i++;
	}
	return (NULL);
}

int execve_error(char **envp, char **temp, char *exe_path)
{
	if (envp)
		free_matrix(envp);
	if (temp)
		free_matrix(temp);
	if (exe_path)
		free(exe_path);
	perror("execve failed");
	return (127);
}

int open_ve_error(t_cmd *cmd, t_shell_state **shell, char *exe_path)
{
	(void)cmd;
	free_command_all((*shell)->current_cmd_list->head);
	free((*shell)->current_cmd_list);
	free_env((*shell)->env_list);
	free(exe_path);
	free(*shell);
	return (EXIT_FAILURE);
}

int open_ve_out(int *docfd, t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->output_files[i + 1])
	{
		ft_printfd(1, "Opening output file: %s\n", cmd->output_files[i]);
		docfd[1] = open(cmd->output_files[i], O_RDWR | O_CREAT | (cmd->append_mode * O_APPEND)
			| (!cmd->append_mode * O_TRUNC),  OUTFILE_PERMS);
		if (docfd[1] < 0)
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_files[i]);
		if (docfd[1] < 0)
			return (-1);
		close(docfd[1]);
		i++;
	}
	docfd[1] = open(cmd->output_file, O_RDWR | O_CREAT | (cmd->append_mode * O_APPEND)
			| (!cmd->append_mode * O_TRUNC),  OUTFILE_PERMS);
	if (docfd[1] < 0)
	{
		ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
		return (-1);
	}
	return (0);
}