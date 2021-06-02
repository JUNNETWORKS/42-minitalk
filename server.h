#ifndef SERVER_H
# define SERVER_H

# include <signal.h>

extern int	g_char;
extern int	g_current_bit;

void	restore_data_from_bit(int bit, pid_t client_pid);
void	sigalrm_handler(int signum);
void	sigusr1_handler(int signum, siginfo_t *info, void *context);
void	sigusr2_handler(int signum, siginfo_t *info, void *context);

#endif
