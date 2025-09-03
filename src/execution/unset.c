
#include "../../includes/minishell.h"

// Function to unset (remove) environment variables
int unset(t_cmd *cmd, t_shell_state **shell)
{
	int i;
	t_env *to_delete;

	if (cmd->arg_count < 2)
		return (SUCCESS);
	i = 1;
	while (cmd->args[i])
	{
		to_delete = find_env((*shell)->env_list, cmd->args[i]);
		if (to_delete)
			delete_env(&((*shell)->env_list), to_delete);
		i++;
	}
	return (SUCCESS);
}

// Function to delete an environment variable from the list
int delete_env(t_env **env, t_env *to_delete)
{
	t_env *current;
	t_env *prev;

	if (!env || !*env || !to_delete)
		return (-1);

	current = *env;
	prev = NULL;

	while (current)
	{
		if (current == to_delete)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->value);
			free(current);
			return (1);
		}
		prev = current;
		current = current->next;
	}
	return (-1);
}
