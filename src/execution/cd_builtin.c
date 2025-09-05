/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:00:12 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/03 18:34:16 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <linux/limits.h>

void	change_env_wd(char *newwd, char *oldwd, t_env *env)
{
	char	*temp;
	
	temp = ft_strjoin("OLDPWD=", oldwd);
	if (!find_env(env, "OLDPWD="))
		add_env(&env, temp);
	else
		update_env(env, temp);
	free (temp);
	temp = ft_strjoin("PWD=", newwd);
	if (!find_env(env, "PWD="))
		add_env(&env, temp);
	else
		update_env(env, temp);
	free(temp);
	return ;
}

int change_dir(char *path, t_env *env)
{
	char	*oldwd;
	char	cwd[PATH_MAX];

	oldwd = NULL;
	if (chdir(path) != 0)
		return (1);
	oldwd = getcwd(cwd, PATH_MAX);
	if (!oldwd)
	{
		ft_printfd(2, "cd: error retrieving current directory");
		return(1);
	}
	else
		oldwd = ft_strdup(cwd);
	
	change_env_wd(path, oldwd, env);
	return (0);
}


int	cd_builtin(t_cmd *cmd, t_shell_state **shell)
{
	char	*path;
	
	path = cmd->args[1];
	if (cmd->arg_count > 2 )
	{
		ft_printfd(2, "minishell: cd: too many arguments\n");
		return (1);
	}
	if (cmd->arg_count == 1 || (cmd->arg_count == 2 && ft_strncmp(cmd->args[1], "~", 2) == 0))
	{
		if (!find_env_val((*shell)->env_list, "HOME"))
			return(1);
		else
			path = find_env_val((*shell)->env_list, "HOME");
	}
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
	{
		if (!find_env_val((*shell)->env_list, "OLDPWD"))
			return(1);
		else
			path = find_env_val((*shell)->env_list, "OLDPWD");
	}
	return (change_dir(path, (*shell)->env_list));
}
char	*find_env_val(t_env *env,char *node)
{
	t_env *temp_env; 
	temp_env = find_env(env, node);
	if (temp_env && temp_env->value)
		return(temp_env->value);
	else
	{
		ft_printfd(2, "minishell: cd: %s not set\n", node);
		return (NULL);
	}	
}