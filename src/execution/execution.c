/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 13:03:03 by lzorzit           #+#    #+#             */
/*   Updated: 2025/08/29 17:14:37 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	inbuilt_e_others()
{
	return (1);
}

// Function to execute a command based on its type
int execute_cmd(t_cmd *cmd, t_env *envar)
{
	int 	fdin;// File descriptor for input redirection
	int 	fdout;// File descriptor for output redirection
	char	*exe_path;			//cw

	fdout = STDOUT_FILENO;
	if(cmd->next != NULL)
	{
		pipeman(cmd, cmd->next, envar);
		return (1);
	}
	if (!cmd || !cmd->args || !cmd->args[0])
		return (-1);
	if (cmd->input_file)
	{
		fdin = open(cmd->input_file, O_RDONLY);
		if (fdin < 0)
		{
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->input_file);
			return (-1);	
		}
	}
	if (cmd->output_file)
	{
		fdout = open(cmd->output_file, O_WRONLY | O_CREAT);
		if (fdout < 0)
		{
			close(fdin);	//cw
			ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->output_file);
			return (-1);	
		}
	}
	if (is_valid_cmd(cmd->args[0]))
		command_select(cmd, fdout, envar);
    else
    {
		exe_path = build_exe_path(envar, cmd);
		if (!exe_path)
		{
			//cleanup
			return (-1);
		}
		execve(exe_path, cmd->args, env_to_matrx(envar));
		if (fdout > 1)
			close(fdout);
		return (-1);
    }
	if (fdout > 1)
		close(fdout);
	return (1);
}


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
int command_select(t_cmd *cmd, int fd, t_env *envar)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (echo(cmd->args, fd));
	// else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
	// 	return (cd_exec(cmd->args));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (pwd(fd));
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (export(cmd, fd, envar));
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (unset(cmd, envar));
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (env(fd, envar, 0));
	// else    if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	// 	return (exit_exec(cmd->args));
	else
	{
		ft_printfd(1, "minishell: %s: command not found\n", cmd->args[0]);
		return (-1);
	}
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

