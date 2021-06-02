#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include "server.h"

int	g_char;
int	g_current_bit;

void	restore_data_from_bit(int bit, pid_t client_pid)
{
	g_char = (g_char << 1) | bit;
	g_current_bit++;
	if (g_current_bit == 8)
	{
		write(STDOUT_FILENO, &g_char, 1);
		g_char = 0;
		g_current_bit = 0;
		kill(client_pid, SIGUSR1);
	}
	sleep(2);  // alarm()代わり
	alarm(2);  // 使用可能関数にはいっていない
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
	struct sigaction	act_sigusr1;
	struct sigaction	act_sigusr2;

	act_sigusr1.sa_flags = SA_SIGINFO;
	act_sigusr1.sa_sigaction = sigusr1_handler;
	if (sigemptyset(&act_sigusr1.sa_mask) < 0 || sigaction(SIGUSR1, &act_sigusr1, NULL) < 0)
		return (-1);
	sigemptyset(&act_sigusr2.sa_mask);
	act_sigusr2.sa_flags = SA_SIGINFO;
	act_sigusr2.sa_sigaction = sigusr2_handler;
	sigaction(SIGUSR2, &act_sigusr2, NULL);
	signal(SIGALRM, sigalrm_handler);
}

// TODO: ALARM()つかっちゃだめ!!
// TODO: グローバル変数は1つまで (レビュー項目にそう書いてある)
// TODO: ちゃんとエラーチェックしましょう
// TODO: *_bonus.c はいらないかなぁ
// TODO: 1文字ごとにwrite()すると実行速度が遅くてsignalが詰まる可能性がある. 大きい配列を持っていおいて, それに格納していって, ヌル文字もしくは配列の大きさ分文字が来たらwrite()する.
// TODO: ACKは1文字ずつではなく, 最後にヌル文字が来た場合に送るようにする
int	main(int argc, char **argv)
{
	pid_t				pid;

	if (argc != 1)
	{
		write(STDERR_FILENO, "argc is invalid\n", -1);  // TODO
		return (1);
	}
	g_char = 0;
	g_current_bit = 0;
	setup_sig_handlers();
	pid = getpid();
	write(STDOUT_FILENO, "server_pid: ", 12);
	print_pid(pid);
	write(1, "\n", 1);
	while (1)
		pause();
	return (0);
}
