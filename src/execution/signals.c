/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:10:15 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/26 11:18:49 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t g_signal_received = 0;

void interactive_sigint_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1); // Va a capo
	rl_on_new_line();			   // Dice a readline: nuova riga
	rl_replace_line("", 0);		   // Pulisce la riga corrente
	rl_redisplay();				   // Ridisegna il prompt
}

void setup_interactive_signals(void)
{
	signal(SIGINT, interactive_sigint_handler);
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ ignorato
}
