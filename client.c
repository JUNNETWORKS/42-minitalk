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

	if (argc < 3)
	{
		printf("You have to pass server_pid and message\n");
		return (1);
	}
	server_pid = parse_pid(argv[1]);
	printf("send |%s| to %d\n", argv[2], server_pid);
	if (kill(server_pid, SIGUSR1) == -1 || kill(server_pid, SIGUSR2) == -1)
	{
		printf("%s\n", strerror(errno));
		return (1);
	}
	return (0);
}
