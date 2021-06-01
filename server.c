#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void	sigusr1_handler(int signum)
{
	printf("receive: SIGUSR1: %d\n", signum);
}

void	sigusr2_handler(int signum)
{
	printf("receive: SIGUSR2: %d\n", signum);
}

void	register_sig_handlers()
{
	signal(SIGUSR1, sigusr1_handler);
	signal(SIGUSR2, sigusr2_handler);
}

int	main()
{
	printf("server pid: %d\n", getpid());

	register_sig_handlers();

	while (1)
		pause();
	return (0);
}
