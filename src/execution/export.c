

#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include "templib.h"

// Function to print the environment variables in sorted order
int export(t_cmd *cmd, int fd, t_env *envar)
{
	int i;

	i = 1;
	if (!cmd->args[2])
		return (env(fd, envar, 1));
	while (cmd->args[i])
	{
		if (ft_isalpha(cmd->args[i][0]) == 0 && cmd->args[i][0] != '_')
		{
			ft_printfd(fd, "minishell: export: `%s': not a valid identifier\n", cmd->args[i]);
			return (-1);
		}// If the variable already exists, update its value, otherwise, add a new variable to the environment list
		if (!find_env(envar, cmd->args[i]))
			add_env(envar, cmd->args[i]);
		else if (find_env(envar, cmd->args[i]))
			update_env(envar, cmd->args[i]);
		i++;
	}
	env(fd, envar, 0);
	return (1);
}
// Function to add a new environment variable to the list
int add_env(t_env *envar, char *arg)
{
	t_env	*new;
	t_env	*temp;

	new = malloc(sizeof(t_env));
	if (!new)
		return (-1);
	new->value = ft_strdup(arg);
	if (!new->value)
	{
		free(new);
		return (-1);
	}
	new->next = NULL;
	if (!envar)
	{
		envar = new;
		return (1);
	}
	temp = envar;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
	return (1);
}
// Function to update the value of an existing environment variable
int update_env(t_env *envar, char *arg)
{
	t_env	*temp;
	char	*copy;
	char	*new_value;
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	copy = ft_substr(arg, 0, i);
	new_value = ft_strdup(arg);
	if (!copy || !new_value)
	{
		free(copy);
		free(new_value);
		return (-1);
	}
	temp =find_env(envar, arg);
	temp->value = new_value;
	free(copy);
	free(new_value);
	return (1);
}
// Function to find an environment variable
t_env *find_env(t_env *envar, char *arg)
{
	t_env	*temp;
	char	*copy;
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	copy = ft_substr(arg, 0, i);
	if (!copy)
		return (NULL);
	temp = envar;
	while (temp)
	{
		if (ft_strncmp(temp->value, copy, i) == 0 &&
			(temp->value[i] == '=' || temp->value[i] == '\0'))
		{
			free(copy);
			return (temp);
		}
		temp = temp->next;
	}
	free(copy);
	return (NULL);
}
