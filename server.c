#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "server.h"

int	g_char;
int	g_current_bit;

void	restore_data_from_bit(int bit, pid_t client_pid)
{
	g_char = (g_char << 1) | bit;
	g_current_bit++;
	alarm(2);
	if (g_current_bit == 8)
	{
		write(STDOUT_FILENO, &g_char, 1);
		g_char = 0;
		g_current_bit = 0;
		usleep(10);
		kill(client_pid, SIGUSR1);
	}
}

void	print_pid(pid_t	pid)
{
	char	c;

	if (!pid)
		return ;
	print_pid(pid / 10);
	c = (pid % 10) + '0';
	write(STDOUT_FILENO, &c, 1);
}

void	setup_sig_handlers(void)
{
	struct sigaction	act_sigusr1;
	struct sigaction	act_sigusr2;

	sigemptyset(&act_sigusr1.sa_mask);
	act_sigusr1.sa_flags = SA_SIGINFO;
	act_sigusr1.sa_sigaction = sigusr1_handler;
	sigaction(SIGUSR1, &act_sigusr1, NULL);
	sigemptyset(&act_sigusr2.sa_mask);
	act_sigusr2.sa_flags = SA_SIGINFO;
	act_sigusr2.sa_sigaction = sigusr2_handler;
	sigaction(SIGUSR2, &act_sigusr2, NULL);
	signal(SIGALRM, sigalrm_handler);
}

int	main(void)
{
	pid_t				pid;

	g_char = 0;
	g_current_bit = 0;
	setup_sig_handlers();
	pid = getpid();
	write(STDOUT_FILENO, "server_pid: ", 12);
	print_pid(pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
