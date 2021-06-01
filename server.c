#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void	restore_data_from_bit(int bit)
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
		c = 0;
		current_bit = 0;
	}
}

void	sigusr1_handler(int signum)
{
	(void)signum;
	restore_data_from_bit(0);
}

void	sigusr2_handler(int signum)
{
	(void)signum;
	restore_data_from_bit(1);
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

	// 文字列を1bitずつclientから送信する
	// SIGUSR1: 0, SIGUSR2: 1 と見立てて受信し, 文字を構成する
	while (1)
		pause();
	return (0);
}
