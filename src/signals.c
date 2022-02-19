/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 07:23:04 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/02/19 13:01:36 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_handle_sigint_in_parent(int signum)
{
	(void)signum;
	g_exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	ms_handle_signals_in_parent(void)
{
	struct sigaction	sigaction_sigint;
	struct sigaction	sigaction_sigquit;

	ms_memset(&sigaction_sigint, 0, sizeof(sigaction_sigint));
	sigaction_sigint.sa_handler = &ms_handle_sigint_in_parent;
	sigaction_sigint.sa_flags = SA_NODEFER;
	if (sigaction(SIGINT, &sigaction_sigint, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
	ms_memset(&sigaction_sigquit, 0, sizeof(sigaction_sigquit));
	sigaction_sigquit.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sigaction_sigquit, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
}

void	ms_ignore_signals(void)
{
	struct sigaction	signal_action;
	struct sigaction	signal_action_2;

	ms_memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &signal_action, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
	ms_memset(&signal_action_2, 0, sizeof(signal_action_2));
	signal_action_2.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &signal_action_2, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
}

void	ms_reset_signals(void)
{
	struct sigaction	signal_action;
	struct sigaction	signal_action_2;

	ms_memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = SIG_DFL;
	if (sigaction(SIGINT, &signal_action, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
	ms_memset(&signal_action_2, 0, sizeof(signal_action_2));
	signal_action_2.sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &signal_action_2, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
}
