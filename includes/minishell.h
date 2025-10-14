/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:32:17 by cwannhed          #+#    #+#             */
/*   Updated: 2025/10/14 11:36:46 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define BOLD				"\033[1m"
# define BLUE				"\033[0;34m"
# define RED				"\033[0;31m"
# define GREEN				"\033[0;32m"
# define RESET				"\033[0m"
# define OUTFILE_PERMS		0644
# define SUCCESS			0
# define FAILURE			1

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <linux/limits.h>
# include <sys/stat.h>

extern volatile sig_atomic_t	g_signal_received;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	QUOTED_STRING,
	VARIABLE
}	t_token_type;

typedef enum e_signal_mode
{
	INTERACTIVE,
	EXECUTION,
	PIPELINE,
	DFL
}	t_signal_mode;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	int				prec_space;
	int				was_quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_token_list
{
	t_token		*head;
	int			count;
}	t_token_list;

typedef enum e_state
{
	DEFAULT,
	IN_SINGLE_QUOTES,
	IN_DOUBLE_QUOTES,
	IN_VARIABLE,
	IN_OPERATOR
}	t_state;

typedef struct s_parser
{
	char		*buffer;
	int			buffer_pos;
	int			space_encountered;
	t_state		state;
}	t_parser;

typedef struct s_tokenizer_ctx
{
	t_parser		parser;
	t_token_list	*tokens;
}	t_tokenizer_ctx;

typedef struct s_cmd
{
	char			**args;
	int				arg_count;
	char			*input_file;
	int				is_heredoc;
	char			*heredoc_delimiter;
	char			**heredoc_delimiters;
	int				heredoc_count;
	int				heredoc_expand;
	char			*output_file;
	int				append_mode;
	char			**input_files;
	int				input_count;
	char			**output_files;
	int				*output_modes;
	int				output_count;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_cmd_list
{
	t_cmd	*head;
	int		count;
}	t_cmd_list;

typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell_state
{
	t_env		*env_list;
	int			last_exit_status;
	int			should_exit;
	int			exit_code;
	int			is_child;
	t_cmd_list	*current_cmd_list;
}				t_shell_state;

/****************MAIN UTILS******************/

int		shell_loop(t_shell_state **shell);
char	*read_input_line(t_shell_state **shell);
int		is_all_spaces(char *input);
int		init_shell_state(t_shell_state *shell, char **envp);

/****************PARSING******************/

//cleanup.c
void	free_token(t_token *token);
void	free_token_list(t_token_list *token_list);
void	free_command_list(t_cmd_list *cmd_list);
void	free_cmd(t_cmd *cmd);
void	free_heredoc_delimiters(char **delimiters);

//token_cleanup.c
void	free_string_array(char **array);

//tokens_to_cmds_handlers.c
int		handle_argument_token(t_token *token, t_cmd *cmd);
int		handle_pipe_token(t_cmd **cmd, t_cmd_list *cmd_list);
int		handle_redirect_token(t_token **curr_token, t_cmd *cmd);

//tokens_to_cmds_operators.c
int		set_input_redirect(t_cmd *cmd,
			char *filename,
			t_token **curr_token);
int		set_output_redirect(t_cmd *cmd,
			char *filename,
			int append,
			t_token **curr_token);
int		set_heredoc_delimiter(t_cmd *cmd,
			char *delimiter,
			t_token **curr_token);

//tokens_to_cmds_operators_utils.c
int		add_input_file(t_cmd *cmd, char *filename);
int		add_output_file(t_cmd *cmd, char *filename, int append);
int		add_heredoc_delimiter(t_cmd *cmd, char *delimiter);
int		allocate_output_arrays(t_cmd *cmd, char ***new_files, int **new_modes);

//tokens_to_cmds_utils.c
int		is_argument_token(t_token *token);
int		is_redirect_token(t_token *token);
int		finalize_last_command(t_cmd *curr_cmd, t_cmd_list *cmd_list);
int		process_token_loop(t_token_list *tokens, t_cmd_list *cmd_list);

//tokens_to_cmds.c
int		tokens_to_commands(t_token_list *tokens, t_cmd_list *commands);
int		process_curr_token(t_token **curr_token,
			t_cmd **curr_cmd,
			t_cmd_list *cmd_list);
int		add_command_to_list(t_cmd *new_cmd, t_cmd_list *cmd_list);

//parser.c
int		parse_input(char *input, t_cmd_list	**cmd_list, t_shell_state **shell);
int		init_and_tokenize(char *input, t_tokenizer_ctx *ctx);
int		finalize_pending_token(t_tokenizer_ctx *ctx);
int		build_cmd_list(t_cmd_list **cmd_list, t_tokenizer_ctx *ctx);

//parser_utils.c
void	add_to_buffer(char c, t_parser *parser);
int		init_tokenizer_ctx(t_tokenizer_ctx *ctx, char *input);
void	reset_buffer(t_parser *parser);
void	cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx);
void	init_cmd(t_cmd *cmd);
t_cmd	*create_cmd(void);

//state_machine_extra.c
int		handle_operator_state(char c, t_tokenizer_ctx *ctx);
int		create_redirect_token(t_tokenizer_ctx *ctx);
int		handle_first_var_char(char c, t_tokenizer_ctx *ctx);
int		handle_more_var_char(char c, t_tokenizer_ctx *ctx);

//state_machine_handlers.c
int		handle_dollar_char(t_tokenizer_ctx *ctx);
int		handle_space_char(t_tokenizer_ctx *ctx);
int		handle_redirect_start(char c, t_tokenizer_ctx *ctx);
int		handle_pipe_char(t_tokenizer_ctx *ctx);

//state_machine.c
int		handle_state_machine(char c, t_tokenizer_ctx *ctx);
int		handle_double_quotes(char c, t_tokenizer_ctx *ctx);
int		handle_single_quotes(char c, t_tokenizer_ctx *ctx);
int		handle_default_state(char c, t_tokenizer_ctx *ctx);
int		handle_variable_state(char c, t_tokenizer_ctx *ctx);

//token_merger.c
int		merge_adjacent_tokens(t_tokenizer_ctx *ctx);
int		should_merge_tokens(t_token *curr, t_token *next);
int		merge_tokens(t_token *curr, t_token *next);

//token_utils.c
int		tokenize(char *input, t_tokenizer_ctx *ctx);
int		safe_create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type);
int		create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type);
void	add_token_list(t_token_list *token_list, t_token *token);
int		last_token_is_pipe(t_token_list *token_list);

//utils.c
int		check_unclosed_quotes(t_parser *parser);

//var_expansion.c
int		expand_variables(t_shell_state *shell, t_token_list *token_list);
int		expand_single_var(t_shell_state *shell, t_token *token);
int		handle_var_in_str(t_shell_state *shell, t_token *token);
char	*process_string_expansion(t_shell_state *shell, char *str);
char	*expand_var_str(t_shell_state *shell, char *str, int *i, char *old_str);

//var_expansion_utils.c
char	*get_env_value(t_shell_state *shell, char *var_name);
char	*extract_var_name(char *str, int *dollar_pos, int *var_len);
char	*append_char(char *old_str, char c);
char	*extract_value_from_env_list(t_env *env, char *var_name);
char	*get_value_from_env_str(char *env_str);

/****************EXECUTION**************** */

// build_exe_path.c
char	*build_exe_path(t_shell_state *shell, t_cmd *cmd);
char	*find_cmd_exe(char **paths, t_cmd *cmd);
int		is_valid_exe_path(const char *path);
// cleanup.c
void	free_matrix(char **matrix);
void	free_command_all(t_cmd *cmd);
int		pipe_free_all(t_cmd *cmd_left, t_shell_state *shell);
int		set_last_exit_status(t_shell_state *shell, int status);

// enviroment
int		update_env(t_env *envar, char *arg);
int		add_env(t_env **envar, char *arg);
t_env	*find_env(t_env *envar, char *arg);
int		env(t_env *env, int fd, int print_all);
t_env	*env_to_list(char **envp);
char	**env_to_matrix(t_env *env);
char	*find_env_val(t_env *env, char *node);

//redirection
int		open_ve_out(int *docfd, t_cmd *cmd);
int		open_ve_doc(int *docfd, t_cmd *cmd);
int		open_ve_error(t_shell_state **shell, char *exe_path);
int		check_param_fd(int fd, va_list arg, char c);

//pipe and fork
int		pipeman(t_cmd *cmd_left, t_cmd *cmd_right, t_shell_state *shell);
int		fork_and_execute(t_cmd *cmd_left,
			int *status,
			t_shell_state *shell,
			int *pipefd);
int		exec_pipeline_left(t_cmd *cmd, t_shell_state *shell, int *fd);
int		exec_pipeline_right(t_cmd *cmd, t_shell_state *shell, int *fd);
int		pipe_error(int *fd);
int		heredoc_status(int *fd, t_shell_state *shell);
int		heredoc_closing(t_cmd *cmd, int *fd);
int		fork_close(int *fd, pid_t *whait1, pid_t *whait2, int *status);
int		fork_error(int *fd, pid_t *whait1, pid_t *whait2, int *status);
char	*expand_in_heredoc(char *line, t_shell_state *shell);

//heredoc
int		set_up_heredoc(t_cmd *cmd, t_shell_state *shell);
char	*get_all_line(int fd);
int		heredoc_read(int *pipefd,
			const char *delimiter,
			t_shell_state *shell,
			int expand);
char	*expand_in_heredoc(char *line, t_shell_state *shell);
int		heredoc_sub(t_cmd *cmd, int *fd, t_shell_state *shell);
int		heredoc_execve(t_cmd *cmd);
int		heredoc_read_placebo(char **delimiter);

// execution
int		execute_cmd(t_cmd *cmd, t_shell_state **shell);
int		is_valid_cmd(char *cmd);
int		command_select(t_cmd *cmd, t_shell_state **shell);
int		ft_printfd(int fd, const char *format, ...);
char	**dup_matrix(char **matrix);
int		handle_builtin(t_cmd *cmd, t_shell_state **shell);
int		handle_external_command(t_cmd *cmd, t_shell_state **shell);
int		is_valid_cd_path(const char *path);

//external command - execve
int		execve_temp(char *exe_path, t_cmd *cmd, t_shell_state **shell);
int		open_ve(t_cmd *cmd);
int		execve_matr_fail(char **envp, char **temp, t_shell_state **shell);
int		execve_error(char **envp, char **temp, char *exe_path);

// inbuilt commands
int		open_placebo_all(t_cmd *cmd, int *fd);
int		pwd(int fd);
int		echo_exec(char **str, int n_var, int fd);
void	free_env(t_env *head);
int		echo_builtin(char **args, int fd);
int		export(t_cmd *cmd, t_shell_state **shell, int fd);
int		unset(t_cmd *cmd, t_shell_state **shell);
int		delete_env(t_env **env, t_env *to_delete);
int		builtin_exit(t_cmd *cmd, t_shell_state *shell);
int		validate_exit_arg(char *arg);
int		change_dir(char *path, t_env *env);
int		cd_builtin(t_cmd *cmd, t_shell_state **shell);
int		open_in_placebo(t_cmd *cmd, int *fd, int i, int bool);
int		set_output(t_cmd *cmd, int *fd);
int		open_in(t_cmd *cmd, int *fd);

// signals
void	heredoc_exit_handler(int sig);
void	setup_signals(int mode);

#endif