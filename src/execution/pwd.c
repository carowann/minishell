

#include "../../includes/minishell.h"
#include <linux/limits.h>

int	pwd(int fd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_printfd(fd, "%s\n", cwd);
		return (SUCCESS);
	}
	else
		return (1);
}
