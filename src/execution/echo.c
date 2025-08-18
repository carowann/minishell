/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:02:22 by lzorzit           #+#    #+#             */
/*   Updated: 2025/08/18 18:05:16 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../libft/libft.h"

//	convert matrix to string, TODO move to another file for use in other function and check memoory leaks
char *conv_to_strn(char	**args)
{
	char	*str; //return
	int		i; //index

	i = 0;
	if (ft_strncmp(args[0], "-n", 3) == 0)
		i++;
	while (args[i])
	{
		str = ft_strjoin(str, args[i]);
		i++;
		if(args[i])
			str = ft_strjoin(str, " ");	
	}
	return(str);
}
//	command called when the Echo keyword is read
int	echo(char	**args, int redirect_fd)
{
	int		n_var;

//	check for -n option, witch skips the terminating /n
	if (args[0] == "-n")
		n_var = 1;
	else 
		n_var = 0;
	if (echo_exec( conv_to_strn(args), n_var, redirect_fd) == 0)
		return(-1);
	return(1);
}

//	execution of command after vars are passed by echo
int	echo_exec(char *str, int n_var, int fd)
{
	int	i;

	i = 0;
	while (str[i]!= '\0')
	{
		write(fd, str[i], 1);
		i++;
	}
	if(n_var == 0)
		write(fd, '\n', 1);
	if(str)
		free(str);
	return(1);
}

// int	main(int argc, char **argv)
// {
// 	echo(argv, 1);
// 	return(0);
// }

