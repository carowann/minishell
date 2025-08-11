/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:32:50 by cwannhed          #+#    #+#             */
/*   Updated: 2025/04/25 15:19:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	size_t	get_n_len(int n, int div)
{
	size_t	len;

	len = 0;
	if (n < 0)
	{
		len++;
		n *= -1;
	}
	while (div > 0)
	{
		len++;
		div /= 10;
	}
	return (len);
}

static size_t	get_div(int n)
{
	size_t	div;

	div = 1;
	if (n < 0)
		n *= -1;
	while (n / div >= 10)
		div *= 10;
	return (div);
}

static	void	ft_itoa_helper(char *str, int n, int div)
{
	if (n < 0)
	{
		*str = '-';
		n *= -1;
		str++;
	}
	while (div >= 1)
	{
		*str = (n / div) + '0';
		n = n % div;
		div /= 10;
		str++;
	}
	*str = '\0';
}

char	*ft_itoa(int n)
{
	char	*str;
	char	*start;
	size_t	len;
	int		div;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	div = get_div(n);
	len = get_n_len(n, div);
	str = (char *)ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	start = str;
	ft_itoa_helper(str, n, div);
	return (start);
}
