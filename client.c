#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int	g_ack_count;

pid_t	parse_pid(char *str)
{
	int	num;

	num = 0;
	while (*str)
	{
		if ('0' <= *str && *str <= '9')
			num = num * 10 + *str - '0';
		else
			return (-1);
		str++;
	}
	return (num);
}

void	sigusr1_handler(int signum)
{
	(void)signum;
	g_ack_count++;
}

int	send_str_by_signal(char *str, pid_t server_pid)
{
	int		i;
	int		bit_pos;

	g_ack_count = 0;
	i = 0;
	while (str[i])
	{
		bit_pos = 7;
		if (i > g_ack_count && sleep(2) == 0)
			return (1);
		while (bit_pos >= 0)
		{
			if (kill(server_pid, (int[]){SIGUSR1, SIGUSR2}[str[i] & (1 << bit_pos)]) < 0)
				return (1);
			usleep(100);
			bit_pos--;
		}
		i++;
	}
	return (0);
}

// TODO: 1000文字2秒以内
// TODO: ACKが届いてからclientはexitする.
int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc != 3)
	{
		write(STDERR_FILENO, "argc is invalid\n", -1);  // TODO
		return (1);
	}
	server_pid = parse_pid(argv[1]);
	if (!(server_pid > 0))
		return (1);
	signal(SIGUSR1, sigusr1_handler);
	return (send_str_by_signal(argv[2], server_pid));
}
