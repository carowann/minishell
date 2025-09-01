/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printfd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzorzit <lzorzit@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:43:56 by lzorzit           #+#    #+#             */
/*   Updated: 2025/09/01 17:22:45 by lzorzit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_param_fd(int fd, va_list arg, char c)
{
	if (c == '%')
		return (ft_putchar_fd('%', fd));
	else if (c == 'c')
		return (ft_putchar_fd((va_arg(arg, int)), fd));
	else if (c == 's')
		return (ft_putstr_fd(va_arg(arg, char *), fd));
	else if (c == 'x')
		return (ft_putnbr_hex_fd(va_arg(arg, unsigned int), fd, 'a'));
	else if (c == 'X')
		return (ft_putnbr_hex_fd(va_arg(arg, unsigned int), fd, 'A'));
	else if (c == 'p')
		return (ft_putptr_fd(va_arg(arg, void *), fd));
	else if (c == 'i' || c == 'd')
		return (ft_putnbr_fd(va_arg(arg, int), fd));
	else if (c == 'u')
		return (ft_putnbr_u_fd(va_arg(arg, unsigned), fd));
	return (0);
}

int	ft_printfd(const int fd, const char *format, ...)
{
	int		count;
	size_t	i;
	va_list	arg;

	i = 0;
	count = 0;
	if (format == NULL)
		return (-1);
	va_start(arg, format);
	while (format[i])
	{
		if (format[i] == '%')
			count += check_param_fd(fd, arg, format[++i]);
		else
		{
			write(fd, &(format[i]), 1);
			count++;
		}
		i++;
	}
	va_end(arg);
	return (count);
}
