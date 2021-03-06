#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void	restore_data_from_bit(int bit, pid_t client_pid)
{
	static int	current_char;
	static int	current_bit;
	static char	buffer[1000];
	static int	len;

	current_char = (current_char << 1) | bit;
	current_bit++;
	if (current_bit == 8)
	{
		buffer[len++] = current_char;
		if (current_char == '\0' || len >= 1000)
		{
			write(STDOUT_FILENO, buffer, len - (current_char == '\0'));
			len = 0;
			if (current_char == '\0')
				kill(client_pid, SIGUSR1);
		}
		current_char = 0;
		current_bit = 0;
	}
}

void	sigusr_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	restore_data_from_bit(signum == SIGUSR2, info->si_pid);
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

int	setup_sig_handlers(void)
{
	struct sigaction	act_sigusr;

	act_sigusr.sa_flags = SA_SIGINFO;
	act_sigusr.sa_sigaction = sigusr_handler;
	if (sigemptyset(&act_sigusr.sa_mask) < 0
		|| sigaction(SIGUSR1, &act_sigusr, NULL) < 0
		|| sigaction(SIGUSR2, &act_sigusr, NULL) < 0)
		return (-1);
	return (0);
}

int	main(void)
{
	pid_t				pid;

	if (setup_sig_handlers() == -1)
		return (1);
	pid = getpid();
	write(STDOUT_FILENO, "server_pid: ", 12);
	print_pid(pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
