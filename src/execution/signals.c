/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:10:15 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/26 14:43:24 by cwannhed         ###   ########.fr       */
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

void execution_sigint_handler(int sig)
{
	write(STDOUT_FILENO, "\n", 1);
	(void)sig;
}

void execution_sigquit_handler(int sig)
{
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	(void)sig;
}

void setup_pipeline_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void setup_execution_signals(void)
{
	signal(SIGINT, execution_sigint_handler);            // Ignora SIGINT nel padre
	signal(SIGQUIT, execution_sigquit_handler); // SIGQUIT stampa messaggio
}

// Ripristina i segnali alle impostazioni di default per i processi figli
void setup_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Handler per il processo figlio che libera memoria prima di terminare
void child_signal_handler(int sig)
{

	exit(128 + sig);
}
