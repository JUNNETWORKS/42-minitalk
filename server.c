#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void	restore_data_from_bit(int bit, pid_t client_pid)
{
	static int	c;
	static int	current_bit;

	c = (c << 1) | bit;
	current_bit++;
	if (current_bit == 8)
	{
		write(STDOUT_FILENO, &c, 1);
		c = 0;
		current_bit = 0;
		usleep(10);
		kill(client_pid, SIGUSR1);
	}
}

void	sigusr1_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	restore_data_from_bit(0, info->si_pid);
}

void	sigusr2_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	restore_data_from_bit(1, info->si_pid);
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

int	main(void)
{
	pid_t				pid;
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
	pid = getpid();
	write(STDOUT_FILENO, "server_pid: ", 12);
	print_pid(pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
