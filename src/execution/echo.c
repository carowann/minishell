/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:02:22 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/01 17:30:26 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/minishell.h"

//	execution of command after vars are passed by echo
int	echo_exec(char **str, int n_var)
{
	int	i;

	i = 1;
	if(n_var == 1)
		i++;
	while(str[i])
	{
		if(str[i][0] != '\0')
			ft_printf("%s", str[i]);
		i++;
		if(str[i] && str[i-1][0] != '\0')
			ft_printf(" ");
	}
	if(n_var == 0)
		write(1, "\n", 1);
	return(1);
}
//	command called when the Echo keyword is read
int	echo(char	**args)
{
	int		n_var;
	
	if (!args[1])
	{
		ft_printf("%s","\n");
		return(1);
	}	//	check for -n option, witch skips the terminating /n
	if (ft_strncmp(args[1], "-n", 3) == 0)
		n_var = 1;
	else 
		n_var = 0;
	if (echo_exec( args, n_var) == 0)
		return(-1);
	return(1);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_cmd cmd;
// 	t_env *envar;

// 	cmd.args = argv;
// 	cmd.input_file = NULL;
// 	envar = env_to_list(envp);
// 	if (argc)
// 		execute_cmd(&cmd, envar);
// 	free_env(envar);
// 	return(0);
// }

