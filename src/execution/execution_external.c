/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_external.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:01:54 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/05 18:06:08 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//TODO: REFACTORARE TUTTO QUESTO FILE

// Function to execute a command using execve in a child process
int	execve_temp(char *exe_path, t_cmd *cmd, t_shell_state **shell)
{ 
	pid_t	pid;
	char	**temp;
	char	**envp;
	int		exit_status;

	pid = fork();
	if (pid < 0)
		perror("Fork failed");
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (open_ve(cmd) == -1)
			exit(open_ve_error(cmd, shell));
		envp = env_to_matrix((*shell)->env_list);
		temp = dup_matrix(cmd->args);
		if (!envp || !temp)
			exit(execve_matr_fail(envp, temp, cmd, shell));
		pipe_free_all(cmd, *shell);
		execve(exe_path, temp, envp);
		exit(execve_error(envp, temp, exe_path));
	}
	waitpid(pid, &exit_status, 0);
	return (set_last_exit_status(*shell, exit_status));
}

int	open_ve(t_cmd *cmd)
{
	int fd[2];
	
	fd[0] = -1;
	fd[1] = -1; //inizializzo a -1 per capire se sono stati aperti altrimenti rischio conditionsl jump
	if (cmd->input_file && cmd->is_heredoc < 1)
		open_ve_doc(fd, cmd);
	else if (cmd->is_heredoc > 1)
		heredoc_execve(cmd);
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

