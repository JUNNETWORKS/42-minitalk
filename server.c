#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void	restore_data_from_bit(int bit, pid_t client_pid)
{
	static int	c;
	static int	current_bit;

	printf("%d ", bit);
	fflush(stdout);
	c = (c << 1) | bit;
	current_bit++;
	if (current_bit == 8)
	{
		printf("\nc: %#x\n", c);
		fflush(stdout);
		write(STDOUT_FILENO, &c, 1);
		printf("\n");
		c = 0;
		current_bit = 0;
		// SIGUSR1 を client に送信
		printf("send ACK(SIGUSR1) to client %d\n", client_pid);
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

void	setup_sigactions(void)
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
}

int	main()
{
	printf("server pid: %d\n", getpid());
	setup_sigactions();
	while (1)
		pause();
	return (0);
}
