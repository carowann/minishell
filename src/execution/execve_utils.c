

#include "../../includes/minishell.h"

int execve_matr_fail(char **envp, char **temp, t_cmd *cmd, t_shell_state **shell)
{
	if (envp)
		free_matrix(envp);
	if (temp)
		free_matrix(temp);
	(void)cmd;
	free_command_all((*shell)->current_cmd_list->head);
	free((*shell)->current_cmd_list);
	free_env((*shell)->env_list);
    return (EXIT_FAILURE);
}
//TODO: check if the "FILE" exists and it's not a directory; potresti segnalare dove va controllato, ci metto poco a correggerlo ma non capisco bene dove fare il check
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

int execve_error(char **envp, char **temp, char *exe_path)
{
	if (envp)
		free_matrix(envp);
	if (temp)
		free_matrix(temp);
	if (exe_path)
		free(exe_path);
	perror("execve failed");
	return (127);
}

int open_ve_error(t_cmd *cmd, t_shell_state **shell, char *exe_path)
{
	(void)cmd;
	free_command_all((*shell)->current_cmd_list->head);
	free((*shell)->current_cmd_list);
	free_env((*shell)->env_list);
	free(exe_path);
	free(*shell);
	return (EXIT_FAILURE);
}

int open_ve_doc(int *docfd, t_cmd *cmd)
{
	if (!cmd->input_file || cmd->is_heredoc > 0)
		return (0);
    docfd[0] = open(cmd->input_file, O_RDONLY);
    if (docfd[0]< 0)
    {
        ft_printfd(1, "minishell: %s: No such file or directory\n", cmd->input_file);
        return (-1);
    }
    dup2(docfd[0], STDIN_FILENO);
    close(docfd[0]);
    return (0);
}
