/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:32:17 by cwannhed          #+#    #+#             */
/*   Updated: 2025/09/02 18:30:51 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define MINISHELL_H

# define BOLD		"\033[1m"
# define BLUE		"\033[0;34m"
# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define RESET		"\033[0m"
# define OUTFILE_PERMS		0644
# define SUCCESS			0

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

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

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	int				prec_space;
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
	char			**args; //array di stringhe null terminated per execve e builtin
	int				arg_count; //numero di args, senza contare null terminator
	char			*input_file; //per input redirect <, if null usa stdin normale
	int				is_heredoc; //1 se usa <<, 0 altri menti. if 1 ignora input file e usa heredoc_delimiter
	char			*heredoc_delimiter;
	char			*output_file; // per out redirect > o >>, if null stdout normale
	int				append_mode; //0 sovrascrivi, 1 append
	struct s_cmd	*next; //prossimo cmd nella pipeline
}	t_cmd;

typedef struct s_cmd_list
{
	t_cmd	*head;
	int		count;
}	t_cmd_list;

typedef struct s_grb_node
{
	void				*ptr;
	struct s_grb_node	*next;
}	t_grb_node;

typedef struct	s_macroenv
{
	struct s_env	*head;
	char			**envp;
}					t_macroenv;

typedef struct	s_env
{
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_shell_state
{
	t_env	*env_list;
	int		last_exit_status;
}			t_shell_state;

/****************MAIN UTILS******************/

char	*read_input_line(void);
int		is_all_spaces(char *input);
int		init_shell_state(t_shell_state *shell, char **envp);

/****************PARSING******************/

//cleanup.c
void	free_token(t_token *token);
void	free_token_list(t_token_list *token_list);
void	free_command_list(t_cmd_list *cmd_list);
void	free_cmd(t_cmd *cmd);

//DEBUG!
void		print_token_list(t_token_list *tokens);
const char *get_token_type_name(t_token_type type);
void		print_cmd_list(t_cmd_list *cmd_list);
void		print_cmd_list_detailed(t_cmd_list *cmd_list);


//tokens_to_cmds_handlers.c
int	handle_argument_token(t_token *token, t_cmd *cmd);
int	handle_pipe_token(t_cmd **cmd, t_cmd_list *cmd_list);
int	handle_redirect_token(t_token **curr_token, t_cmd *cmd);

//tokens_to_cmds_operators.c
int	set_input_redirect(t_cmd *cmd, char *filename, t_token **curr_token);
int	set_output_redirect(t_cmd *cmd, char *filename, int append, t_token **curr_token);
int	set_heredoc_delimiter(t_cmd *cmd, char *delimiter, t_token **curr_token);

//tokens_to_cmds_utils.c
int	is_argument_token(t_token *token);
int	is_redirect_token(t_token *token);
int	cleanup_and_return_error(t_cmd *cmd);

//tokens_to_cmds.c
int	tokens_to_commands(t_token_list *tokens, t_cmd_list *commands);
int	process_curr_token(t_token **curr_token, t_cmd **curr_cmd, t_cmd_list *cmd_list);
int	add_command_to_list(t_cmd *new_cmd, t_cmd_list *cmd_list);

//parser.c
int	parse_input(char *input, t_cmd_list	**cmd_list, t_shell_state **shell);
int	init_and_tokenize(char *input, t_tokenizer_ctx *ctx);
int	finalize_pending_token(t_tokenizer_ctx *ctx);
int	build_cmd_list(t_cmd_list **cmd_list, t_tokenizer_ctx *ctx);

//parser_utils.c
void	add_to_buffer(char c, t_parser *parser);
int		init_tokenizer_ctx(t_tokenizer_ctx *ctx, char *input);
void	reset_buffer(t_parser *parser);
void	cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx);

//state_machine_operators.c
int	handle_operator_state(char c, t_tokenizer_ctx *ctx);
int	create_redirect_token(t_tokenizer_ctx *ctx);

//state_machine_variables.c
int	handle_variable_state(char c, t_tokenizer_ctx *ctx);
int	handle_first_var_char(char c, t_tokenizer_ctx *ctx);
int	handle_more_var_char(char c, t_tokenizer_ctx *ctx);

//state_machine.c
int handle_state_machine(char c, t_tokenizer_ctx *ctx);
int	handle_default_state(char c, t_tokenizer_ctx *ctx);
int	handle_double_quotes(char c, t_tokenizer_ctx *ctx);
int	handle_single_quotes(char c, t_tokenizer_ctx *ctx);

//token_merger.c
int	merge_adjacent_tokens(t_tokenizer_ctx *ctx);
int	should_merge_tokens(t_token *curr, t_token *next);
int	merge_tokens(t_token *curr, t_token *next);

//token_utils.c
int		tokenize(char *input, t_tokenizer_ctx *ctx);
int 	safe_create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type);
int		create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type);
void	add_token_list(t_token_list *token_list, t_token *token);
int 	last_token_is_pipe(t_token_list *token_list);

//var_expansion.c
int		expand_variables(t_shell_state *shell, t_token_list *token_list);
int		expand_single_var(t_shell_state *shell, t_token *token);
int		handle_var_in_str(t_shell_state *shell, t_token *token);
char	*process_string_expansion(t_shell_state *shell, char *str);
char	*expand_var_in_str(t_shell_state *shell, char *str, int *i, char *old_str);

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

// cleanup.c
void	free_matrix(char **matrix);

// enviroment
int		update_env(t_env *envar, char *arg);
int		add_env(t_env **envar, char *arg);
t_env	*find_env(t_env *envar, char *arg);
int		env(t_env *env, int fd, int print_all);
t_env	*env_to_list(char **envp);

// execution
int		check_param_fd(int fd, va_list arg, char c);
int		execute_cmd(t_cmd *cmd, t_shell_state **shell);
int		is_valid_cmd(char *cmd);
int		command_select(t_cmd *cmd, t_shell_state **shell);
int		ft_printfd(int fd, const char *format, ...);
char	*conv_to_strn(char **args);
int		pipeman(t_cmd *cmd_left, t_cmd *cmd_right, t_shell_state *shell);
char	**env_to_matrx(t_env *env);
int		exec_pipeline(t_cmd *cmd, t_shell_state *shell, int *fd, int flag);
char	*read_line(void);
int		open_ve(t_cmd *cmd);
int		open_in(t_cmd *cmd, int *fd);
int		execve_temp(char *exe_path, t_cmd *cmd, t_env *env);
void	free_command_all(t_cmd *cmd);
char 	**dup_matrix(char **matrix);
int		pipe_free_all(t_cmd *cmd_left, t_cmd *cmd_right, t_shell_state *shell);

// inbuilt commands
int		pwd(int fd);
int		echo_exec(char **str, int n_var, int fd);
void	free_env(t_env *head);
int		echo(char **args, int fd);
int		export(t_cmd *cmd, t_shell_state **shell, int fd);
int		unset(t_cmd *cmd, t_shell_state **shell);
int		delete_env(t_env **env, t_env *to_delete);
int		builtin_exit(t_cmd *cmd, t_shell_state *shell);
int		validate_exit_arg(char *arg);

#endif