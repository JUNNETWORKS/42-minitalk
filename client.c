#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

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

int	main(int argc, char **argv)
{
	pid_t	server_pid;
	int		i;
	int		bit_pos;
	char	*str;

	if (argc < 3)
		return (1);
	server_pid = parse_pid(argv[1]);
	str = argv[2];
	printf("send |%s| to %d\n", str, server_pid);
	// 文字列を1bitずつserverに送信する
	// SIGUSR1: 0, SIGUSR2: 1 と見立てて送信する
	i = 0;
	while (str[i])
	{
		printf("c: %#x\n", str[i]);
		bit_pos = 7;
		while (bit_pos >= 0)
		{
			printf("%d ", (str[i] & (1 << bit_pos)) != 0);
			fflush(stdout);
			if (str[i] & (1 << bit_pos))
			{
				if (kill(server_pid, SIGUSR2) < 0)
					return (1);
			}
			else
			{
				if (kill(server_pid, SIGUSR1) < 0)
					return (1);
			}
			usleep(50);
			bit_pos--;
		}
		i++;
	}
	return (0);
}
