/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_exe_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 16:29:56 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/02 18:30:09 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*build_exe_path(t_shell_state *shell, t_cmd *cmd)
{
	char	*exe_path;
	char	**all_exe_paths;
	char	*value_path_var;
	
	value_path_var = get_env_value(shell, "PATH");
	if (!value_path_var)
	{
		ft_putstr_fd("PATH environment variable not set\n", STDERR_FILENO);
		return (NULL);
	}
	all_exe_paths = ft_split(value_path_var, ':');
	if (!all_exe_paths)
	{
		free(value_path_var);
		return (NULL);
	}
	free(value_path_var);
	exe_path = find_cmd_exe(all_exe_paths, cmd);
	free_matrix(all_exe_paths);
	return(exe_path);
}

char	*find_cmd_exe(char **paths, t_cmd *cmd)
{
	size_t	i;
	char	*exe_path;

	i = 0;
	if (!cmd || !cmd->args[0] || !cmd->args[0][0])
		return (NULL);
	if (ft_strchr(cmd->args[0], '/') && access(cmd->args[0], X_OK) == SUCCESS)
		return (ft_strdup(cmd->args[0]));
	while (paths[i])
	{
		exe_path = ft_strjoin3(paths[i], "/", cmd->args[0]);
		if (!exe_path)
			return (NULL);
		if (access(exe_path, X_OK) == SUCCESS)
			return (exe_path);
		free(exe_path);
		i++;
	}
	return (NULL);
}
