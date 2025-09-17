#include "../includes/minishell.h"

void set_last_exit_status(t_shell_state *shell, int status)
{
    if (WIFEXITED(status))
        shell->last_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->last_exit_status = 128 + WTERMSIG(status);
    else
        shell->last_exit_status = 1;
    return;
}