#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "server.h"

void	sigalrm_handler(int signum)
{
	(void)signum;
	g_char = 0;
	g_current_bit = 0;
}

void	sigusr1_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	restore_data_from_bit(signum == SIGUSR2, info->si_pid);
}

void	sigusr2_handler(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	(void)context;
	restore_data_from_bit(1, info->si_pid);
}
