
#include "../../libft/libft.h"
#include "../../includes/minishell.h"
#include <linux/limits.h>
#include "templib.h"

int		pwd(int fd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX))
	{
		ft_printfd(fd, "%s\n", cwd);
		return (1);
	}
	else
		return (-1);
}