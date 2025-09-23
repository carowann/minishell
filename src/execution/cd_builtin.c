/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:00:12 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/23 12:54:57 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <linux/limits.h>

//TODO cwannhed@c1r6p8:~$ cd hdsaldjsl
//bash: cd: hdsaldjsl: No such file or directory

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
	char	*currentcwd;
	char	cwd[PATH_MAX];

	oldwd = NULL;
	oldwd = getcwd(cwd, PATH_MAX);
	if (chdir(path) != 0)
		return (1);
	if (!oldwd)
	{
		ft_printfd(2, "cd: error retrieving current directory");
		return(1);
	}
	else
		oldwd = ft_strdup(cwd);
	currentcwd= getcwd(cwd, PATH_MAX);
	change_env_wd(currentcwd, oldwd, env);
	free(oldwd);
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
		if (!find_env_val((*shell)->env_list, "HOME="))
			return(1);
		else
			path = find_env_val((*shell)->env_list, "HOME=");

	}
	else if (ft_strncmp(cmd->args[1], "-", 2) == 0)
	{
		if (!find_env_val((*shell)->env_list, "OLDPWD="))
			return(1);
		else
			path = find_env_val((*shell)->env_list, "OLDPWD");
	}
	return (change_dir(path, (*shell)->env_list));
}

char	*find_env_val(t_env *env,char *node)
{
	t_env *temp_env; 
	char *ret;

	temp_env = find_env(env, node);
	if (temp_env && temp_env->value)
	{
		ret = ft_strrchr(temp_env->value, '=');
		ret ++;
		return(ret);
	}
	else
	{
		ft_printfd(2, "minishell: cd: %s not set\n", node);
		return (NULL);
	}
}
