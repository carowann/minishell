/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:00:00 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/07 12:05:41 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_unclosed_quotes(t_parser *parser)
{
	if (parser->state == IN_SINGLE_QUOTES || parser->state == IN_DOUBLE_QUOTES)
	{
		ft_printfd(2, "minishell: syntax error: unterminated quoted string\n");
		return (-1);
	}
	return (0);
}
