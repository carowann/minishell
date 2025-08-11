/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_hex_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:26:02 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_hex_fd(unsigned int n, int fd, char letter_case)
{
	int		count;
	char	hex_start;
	char	digit;

	if (n == 0)
		return (ft_putchar_fd('0', fd));
	hex_start = 'a';
	if (ft_isupper(letter_case))
		hex_start = 'A';
	count = 0;
	if (n >= 16)
		count += ft_putnbr_hex_fd(n / 16, fd, letter_case);
	if (n % 16 < 10)
		digit = '0' + (n % 16);
	else
		digit = hex_start + (n % 16 - 10);
	count += ft_putchar_fd(digit, fd);
	return (count);
}
