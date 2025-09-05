/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_external.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:01:54 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/05 12:47:44 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: REFACTORARE TUTTO QUESTO FILE

// Function to execute a command using execve in a child process
int	execve_temp(char *exe_path, t_cmd *cmd, t_env *env)
{ 
	pid_t	pid;
	char	**temp;
	char	**envp;
	int		exit_status;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		if (open_ve(cmd) == -1)
		{
			free_command_all(cmd);
			free_env(env);
			exit(EXIT_FAILURE);
		}
		envp = env_to_matrx(env);
		temp = dup_matrix(cmd->args);
		if (!envp || !temp)
		{
			if (envp)
				free_matrix(envp);
			if (temp)
				free_matrix(temp);
			free_command_all(cmd);
			free_env(env);
			exit(EXIT_FAILURE);
		}
		execve(exe_path, temp, envp);
		free_matrix(envp);
		free_matrix(temp);
		free_command_all(cmd);
		free_env(env);
		perror("execve failed");
		exit(127);
	}
	else
	{
		waitpid(pid, &exit_status, 0);
		if (WIFEXITED(exit_status)) //se termina normalmente
			return (WEXITSTATUS(exit_status)); //estrae exit status
		else if (WIFSIGNALED(exit_status)) //se proc uscito da signal
			return (128 + WTERMSIG(exit_status)); // bash restituisce 128 + num segnale
		else
			return (1);
	}
	return (0);
}

int	open_ve(t_cmd *cmd)
{
	int fd[2];

	fd[0] = -1;
	fd[1] = -1; //inizializzo a -1 per capire se sono stati aperti altrimenti rischio conditionsl jump
	if (cmd->input_file)
	{
		fd[0] = open(cmd->input_file, O_RDONLY);
		if (fd[0]< 0)
		{
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->input_file);
			return (-1);	
		}
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	if (cmd->output_file)
	{
		fd[1] = open(cmd->output_file, O_RDWR | O_CREAT | (cmd->append_mode * O_APPEND) 
			| (!cmd->append_mode * O_TRUNC),  OUTFILE_PERMS);
		if (fd[1] < 0)	
		{
			if(fd[0] >= 0) //cambiato perche anche =0 e' un fd valido da chiudere
				close(fd[0]);
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
			return (-1);	
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	return (0);
}

char	*build_exe_path(t_shell_state *shell, t_cmd *cmd)
{
	char	*exe_path;
	char	**all_exe_paths;
	char	*value_path_var;
	
	value_path_var = get_env_value(shell, "PATH");
	if (!value_path_var || ft_strlen(value_path_var) == 0)
	{
		ft_putstr_fd("PATH environment variable not set\n", STDERR_FILENO);
		free(value_path_var);
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
