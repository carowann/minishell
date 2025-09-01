

#include "../../includes/minishell.h"
#include <linux/limits.h>

int		pwd()
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_printf("%s\n", cwd);
		return (1);
	}
	else
		return (-1);
}
