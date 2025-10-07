/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_cancellare.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 10:00:00 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/07 12:17:26 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//TODO:maybe eliminate this file
// extern int g_signal_received;

// void	handle_sigint_interactive(int sig)
// {
// 	(void)sig;

// 	g_signal_received = SIGINT;
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void	handle_sigint_executing(int sig)
// {
// 	(void)sig;

// 	g_signal_received = SIGINT;
// 	ft_putstr_fd("\n", STDOUT_FILENO);
// }

// void	handle_sigint_heredoc(int sig)
// {
// 	(void)sig;

// 	g_signal_received = SIGINT;
// 	// Don't print newline here - let the parent handle display
// 	exit(130); // Exit immediately with proper code
// }


// void	setup_signals_interactive(void)
// {
// 	signal(SIGINT, handle_sigint_interactive);
// 	signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT in interactive mode
// 	g_signal_received = 0; // Reset signal flag when entering interactive mode
// }

// void	setup_signals_executing(void)
// {
// 	signal(SIGINT, handle_sigint_executing);
// 	signal(SIGQUIT, SIG_DFL); // Default SIGQUIT behavior during execution
// }


// void	setup_signals_heredoc(void)
// {
// 	signal(SIGINT, handle_sigint_heredoc);
// 	signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT in heredoc
// }


// void	setup_signals_default(void)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// }

// int	check_signal_exit_status(void)
// {
// 	if (g_signal_received == SIGINT)
// 	{
// 		g_signal_received = 0;
// 		return (130); // Standard exit code for SIGINT
// 	}
// 	return (0);
// }