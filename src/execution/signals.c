/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:10:15 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/26 18:03:43 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_signal_received = 0;

void simple_newline_handler(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	(void)sig;
}

void quit_message_handler(int sig)
{
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	(void)sig;
}

void exit_with_signal_handler(int sig)
{
	exit(128 + sig);
}

void interactive_prompt_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void setup_interactive_signals(void)
{
	signal(SIGINT, interactive_prompt_handler);
	signal(SIGQUIT, SIG_IGN);
}

void setup_execution_signals(void)
{
	signal(SIGINT, simple_newline_handler);
	signal(SIGQUIT, quit_message_handler);
}

void setup_pipeline_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

// void setup_heredoc_signals(void)
// {
// 	signal(SIGINT, exit_with_signal_handler);
// 	signal(SIGQUIT, exit_with_signal_handler);
// }

void setup_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
