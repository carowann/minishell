/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/02 17:52:40 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Function to execute a command based on its type
int execute_cmd(t_cmd *cmd, t_shell_state **shell)
{
	char *exe_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);

	if (cmd->next != NULL)
		return (pipeman(cmd, cmd->next, *shell));

	if (is_valid_cmd(cmd->args[0]))
		command_select(cmd, shell);
	else
	{
		exe_path = build_exe_path(*shell, cmd);
		if (!exe_path)
			return (-1);
		execve_temp(exe_path, cmd, (*shell)->env_list);
	}
	return (1);
}
// Function to execute a command using execve in a child process
int	execve_temp(char *exe_path, t_cmd *cmd, t_env *env)
{
	pid_t	pid;
	char	**temp;
	char	**envp;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		open_ve(cmd);
		envp = env_to_matrx(env);
		temp = dup_matrix(cmd->args);
		free_command_all(cmd);
		free_env(env);
		execve(exe_path, temp, envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}
int	open_ve(t_cmd *cmd)
{
	int fd[2];
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
			if(fd[0] > 0)
				close(fd[0]);
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
			return (-1);	
		}
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	return (-1);
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

int command_select(t_cmd *cmd, t_shell_state **shell)
{
	int fd[1];
	open_in(cmd, fd);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		echo(cmd->args, fd[0]);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		pwd(fd[0]);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		export(cmd, shell, fd[0]); // Passa shell invece di env_list
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		unset(cmd, shell);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		env((*shell)->env_list, fd[0], 0);
	else if (ft_strncmp(cmd->args[0], "exit", 4) == SUCCESS)
		builtin_exit(cmd, *shell);
	else
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
	if (fd[0] != STDOUT_FILENO)
		close(fd[0]);
	return(1);
}

int open_in(t_cmd *cmd,	 int *fd)
{
	*fd = STDOUT_FILENO;
	if (*fd < 0)
	{
		ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
		return (-1);	
	}
	if (cmd->output_file)
	{

		if (cmd->append_mode)
			*fd = open(cmd->output_file, O_RDWR | O_CREAT | O_APPEND, OUTFILE_PERMS);
		else
			*fd = open(cmd->output_file, O_RDWR | O_CREAT | O_TRUNC, OUTFILE_PERMS);
		if (*fd < 0)
		{
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
			return (-1);	
		}
	}
	return (1);
}
