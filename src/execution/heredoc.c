#include "../../includes/minishell.h"

// Reads lines from stdin until 'delimiter' is found, writes them to a pipe
int handle_heredoc(const char *delimiter, int *heredoc_fd)
{
    int pipefd[2];
    char *line;

    if (pipe(pipefd) == -1)
        return (-1);

    while (1)
    {
        ft_putstr_fd("> ", 1); // Prompt
        line = get_next_line(0); // Read from stdin
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        free(line);
    }
    close(pipefd[1]);
    *heredoc_fd = pipefd[0]; // Return read end
    return (0);
}