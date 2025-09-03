/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/03 16:29:42 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int set_exit_status(t_shell_state **shell, int status);
static int handle_pipeline(t_cmd *cmd, t_shell_state **shell);
static int handle_builtin(t_cmd *cmd, t_shell_state **shell);
static int handle_external_command(t_cmd *cmd, t_shell_state **shell);

// Function to execute a command based on its type
int execute_cmd(t_cmd *cmd, t_shell_state **shell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (set_exit_status(shell, 1));
	if (cmd->next != NULL)
		return (handle_pipeline(cmd, shell));
	if (is_valid_cmd(cmd->args[0]))
		return (handle_builtin(cmd, shell));
	return (handle_external_command(cmd, shell));
}

static int set_exit_status(t_shell_state **shell, int status)
{
	(*shell)->last_exit_status = status;
	return (status);
}

static int handle_pipeline(t_cmd *cmd, t_shell_state **shell)
{
	int result;

	result = pipeman(cmd, cmd->next, *shell);
	(*shell)->last_exit_status = result;
	return (result);
}

static int handle_builtin(t_cmd *cmd, t_shell_state **shell)
{
	int result;

	result = command_select(cmd, shell);
	if (ft_strncmp(cmd->args[0], "exit", 5) != 0)
		(*shell)->last_exit_status = result;
	return (result);
}

static int handle_external_command(t_cmd *cmd, t_shell_state **shell)
{
	char *exe_path;
	int result;

	exe_path = build_exe_path(*shell, cmd);
	if (!exe_path)
	{
		ft_printfd(STDERR_FILENO, "minishell: %s: command not found\n", cmd->args[0]);
		return (set_exit_status(shell, 127));
	}
	result = execve_temp(exe_path, cmd, (*shell)->env_list);
	free(exe_path);
	return (set_exit_status(shell, result));
}

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
	return (0);
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
	int	fd[1];
	int	result;
	
	if (open_in(cmd, fd) == -1)
		return (1);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		result = echo(cmd->args, fd[0]);
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
	else
	{
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
		result = 127;
	}
	if (fd[0] != STDOUT_FILENO)
		close(fd[0]);
	return (result);
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
