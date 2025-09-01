/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/01 18:33:19 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <sys/wait.h>

// Function to execute a command based on its type
int execute_cmd(t_cmd *cmd, t_shell_state *shell)
{
	char *exe_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	if(cmd->next != NULL)
		return (pipeman(cmd, cmd->next, shell->env_list));
	if (is_valid_cmd(cmd->args[0]))
		command_select(cmd, shell->env_list);
    else
	{
		exe_path = build_exe_path(shell, cmd);
		if (!exe_path)
		{
			//cleanup
			return (-1);
		}
		execve_temp(exe_path, cmd, env_to_matrx(shell->env_list));
	}
	return (1);
}

int	execve_temp(char *exe_path, t_cmd *cmd, char **env)
{
	pid_t	pid;
	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		fd_open(cmd);
		execve(exe_path, cmd->args, env);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}
int	fd_open(t_cmd *cmd)
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
	return (1);
}
// Function to read a line from standard input
char *read_line(void)
{
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return NULL;
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0'; // Remove newline
    return strdup(buffer); // Allocate and return the string
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
int command_select(t_cmd *cmd, t_env *envar)
{
	pid_t	pid;

	pid = fork();
	if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		return (1);
	}
	fd_open(cmd);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		echo(cmd->args);
	// else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
	// 	return (cd_exec(cmd->args));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		export(cmd, envar);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		unset(cmd, envar);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		env(envar, 0);
	// else    if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	// 	return (exit_exec(cmd->args));
	else
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
	exit(0);
}

char *conv_to_strn(char	**args)
{
	char	*str; //return
	int		i; //index

	i = 0;
	if (!args[i])
		return (ft_strdup(""));
	if(args[i])
		str = ft_strndup(args[i], ft_strlen(args[i]));
	i++;
	if(args[i])
		str = ft_strjoin(str, " ");	
	while (args[i])
	{
		str = ft_strjoin(str, args[i]);
		i++;
		if(args[i])
			str = ft_strjoin(str, " ");	
	}
	return(str);
}

char **env_to_matrx(t_env *env)
{
	char	**matrix;
	t_env	*copy;
	int		i;

	i = 0;
	copy = env;
	while (copy)
	{
		i++;
		copy = copy->next;
	}
	matrix = malloc(sizeof(char *) * (i + 1));
	if (!matrix)
		return (NULL);
	matrix[i] = NULL;
	copy = env;
	i = 0;
	while (copy)
	{
		matrix[i] = ft_strdup(copy->value);
		i++;
		copy = copy->next;
	}
	return (matrix);
}

