/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:02:22 by lzorzit           #+#    #+#             */
/*   Updated: 2025/08/19 18:34:25 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../libft/libft.h"
# include "templib.h"
//	convert matrix to string, TODO move to another file for use in other function
char *conv_to_strn(char	**args)
{
	char	*str; //return
	int		i; //index

	i = 1;
	if (ft_strncmp(args[1], "-n", 3) == 0)
		i++;
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
//	execution of command after vars are passed by echo
int	echo_exec(char *str, int n_var, int fd)
{
	int	i;

	i = 0;
	ft_printfd(fd, "%s",str);
	if(n_var == 0)
		write(fd, "\n", 1);
	if(str)
		free(str);
	return(1);
}
//	command called when the Echo keyword is read
int	echo(char	**args, int redirect_fd)
{
	int		n_var;

	if (!args[1])
	{
		ft_printfd(redirect_fd, "%s","\n");
		return(-1);
	}
//	check for -n option, witch skips the terminating /n
	if (ft_strncmp(args[1], "-n", 3) == 0)
		n_var = 1;
	else 
		n_var = 0;
	if (echo_exec( conv_to_strn(args), n_var, redirect_fd) == 0)
		return(-1);
	return(1);
}

int	main(int argc, char **argv)
{
	if (argc)
		echo(argv, 1);
	return(0);
}

