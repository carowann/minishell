/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:18:14 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_param(va_list arg, char c)
{
	if (c == '%')
		return (ft_putchar_fd('%', 1));
	else if (c == 'c')
		return (ft_putchar_fd((va_arg(arg, int)), 1));
	else if (c == 's')
		return (ft_putstr_fd(va_arg(arg, char *), 1));
	else if (c == 'x')
		return (ft_putnbr_hex_fd(va_arg(arg, unsigned int), 1, 'a'));
	else if (c == 'X')
		return (ft_putnbr_hex_fd(va_arg(arg, unsigned int), 1, 'A'));
	else if (c == 'p')
		return (ft_putptr_fd(va_arg(arg, void *), 1));
	else if (c == 'i' || c == 'd')
		return (ft_putnbr_fd(va_arg(arg, int), 1));
	else if (c == 'u')
		return (ft_putnbr_u_fd(va_arg(arg, unsigned), 1));
	return (0);
}

int	ft_printf(const char *format, ...)
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
			count += check_param(arg, format[++i]);
		else
		{
			write(1, &(format[i]), 1);
			count++;
		}
		i++;
	}
	va_end(arg);
	return (count);
}
