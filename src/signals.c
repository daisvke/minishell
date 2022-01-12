#include "minishell.h"

void	ms_handle_sigint(int signum)
{
	(void)signum;
	rl_replace_line("", 0);
	printf("\n");
	rl_on_new_line();
	rl_redisplay();
}

void	ms_handle_sigquit(int signum)
{
	(void)signum;
	struct termios orig_termios_p;
	struct termios new_termios_p;

// errno is set
	if (tcgetattr(STDIN_FILENO, &orig_termios_p) != MS_SUCCESS)
		exit(EXIT_FAILURE);
	new_termios_p = orig_termios_p;
	new_termios_p.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios_p) != MS_SUCCESS)
		exit(EXIT_FAILURE);
}

void	ms_handle_signals(t_ms *ms_env)
{
	struct sigaction	signal_action;
	struct sigaction	signal_action2;

	signal_action.sa_handler = &ms_handle_sigint;
	//errors
	sigaction(SIGINT, &signal_action, NULL);
	signal_action2.sa_handler = &ms_handle_sigquit;
	sigaction(SIGQUIT, &signal_action2, NULL);
}

