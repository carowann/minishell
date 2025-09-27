/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_external.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 12:01:54 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/27 19:00:05 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		setup_default_signals();
		if (open_ve(cmd) == -1)
			exit(open_ve_error(cmd, shell, exe_path));
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
	fd[1] = -1; //inizializzo a -1 per capire se sono stati aperti altrimenti rischio conditional jump
	if (open_ve_doc(fd, cmd) == -1)
		return (-1);
	else if (cmd->is_heredoc > 1)
		heredoc_execve(cmd);
	if (cmd->output_file)
	{
		if (open_ve_out(fd, cmd) == -1)
			return (-1);
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

int is_valid_exe_path(const char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (127);
	if (access(path, X_OK) != 0)
		return (127);
	if (S_ISDIR(statbuf.st_mode))
		return (126);
	return (SUCCESS);
}

int open_ve_doc(int *docfd, t_cmd *cmd)
{
	int i;

	i = 0;
	if (!cmd->input_file)
		return (0);
	while (i < cmd->input_count - 1)
	{
		docfd[0] = open(cmd->input_files[i], O_RDONLY);
		if (docfd[0] < 0)
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->input_files[i]);
		if (docfd[0] < 0)
			return (-1);
		close(docfd[0]);
		i++;
	}
    docfd[0] = open(cmd->input_file, O_RDONLY);
    if (docfd[0] < 0)
    {
        ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->input_file);
        return (-1);
    }
	if (cmd->is_heredoc == 0)
		dup2(docfd[0], STDIN_FILENO);
    close(docfd[0]);
    return (0);
}

