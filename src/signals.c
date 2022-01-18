/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtanigaw <dtanigaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 07:23:04 by dtanigaw          #+#    #+#             */
/*   Updated: 2022/01/18 13:26:21 by dtanigaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_handle_sigint(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	ms_handle_sigquit(int signum)
{
	struct termios	orig_termios_p;
	struct termios	new_termios_p;

	(void)signum;
	tcgetattr(STDIN_FILENO, &orig_termios_p);
	new_termios_p = orig_termios_p;
	new_termios_p.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_termios_p);
	rl_clear_history();
}

void	ms_handle_signals(void)
{
	struct sigaction	signal_action;
	struct sigaction	signal_action_2;

	ms_memset(&signal_action, 0, sizeof(signal_action));
	signal_action.sa_handler = &ms_handle_sigint;
	if (sigaction(SIGINT, &signal_action, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
	ms_memset(&signal_action_2, 0, sizeof(signal_action_2));
	signal_action_2.sa_handler = &ms_handle_sigquit;
	if (sigaction(SIGQUIT, &signal_action_2, NULL) != MS_SUCCESS)
		ms_print_error_message(14);
}
