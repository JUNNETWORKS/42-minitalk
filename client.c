#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

bool	g_has_received_ack;

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

void	sigusr_handler(int signum)
{
	(void)signum;
	g_has_received_ack = true;
	write(STDOUT_FILENO, "Receinved ACK!\n", 15);
}

int	send_str_by_signal(char *str, pid_t server_pid)
{
	int		i;
	int		bit_pos;

	i = 0;
	while (1)
	{
		bit_pos = 7;
		printf("%c: ", str[i]);
		while (bit_pos >= 0)
		{
			printf("%d ", (str[i] & (1 << bit_pos)) > 0);
			if (kill(server_pid, (int[]){SIGUSR1, SIGUSR2}[(str[i] & (1 << bit_pos)) > 0]) < 0)
				return (1);
			if (usleep(100) == -1)
				return (1);
			bit_pos--;
		}
		printf("\n");
		if (str[i] == '\0')
			break;
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc != 3)
	{
		write(STDERR_FILENO, "argc is invalid\n", 16);
		return (1);
	}
	server_pid = parse_pid(argv[1]);
	if (server_pid <= 0)
	{
		write(STDERR_FILENO, "pid is invalid\n", 15);
		return (1);
	}
	g_has_received_ack = false;
	if (signal(SIGUSR1, sigusr_handler) == SIG_ERR
		|| send_str_by_signal(argv[2], server_pid) != 0)
		return (1);
	if (!g_has_received_ack)
		sleep(5);
	if (g_has_received_ack)
		return (0);
	write(STDERR_FILENO, "Server hasn't responded to client\n", 34);
	return (1);
}
