/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:10:15 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/14 11:02:21 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	simple_newline_handler(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	(void)sig;
}

void	quit_message_handler(int sig)
{
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	(void)sig;
}

void	interactive_prompt_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	heredoc_exit_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

/*
 * Unified signal setup function
 * @param mode: signal configuration mode
 */
void	setup_signals(int mode)
{
	if (mode == INTERACTIVE)
	{
		signal(SIGINT, interactive_prompt_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == EXECUTION)
	{
		signal(SIGINT, simple_newline_handler);
		signal(SIGQUIT, quit_message_handler);
	}
	else if (mode == PIPELINE)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (mode == DFL)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
}
