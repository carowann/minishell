/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:32:17 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/19 17:56:42 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MINISHELL_H
# define MINISHELL_H

# define BOLD	"\033[1m"
# define RESET	"\033[0m"

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

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
	char			*buffer;
	int				buffer_pos;
	t_state			state;
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

/****************PARSING**************** */

//cleanup.c
void	free_token(t_token *token);
void	free_token_list(t_token_list *token_list);
void	free_command_list(t_cmd_list *cmd_list);
void	free_cmd(t_cmd *cmd);

//DEBUG!
void		print_token_list(t_token_list *tokens);
const char *get_token_type_name(t_token_type type);
void		print_cmd_list(t_cmd_list *cmd_list);

//operator_state_handler.c
int	handle_operator_state(char c, t_tokenizer_ctx *ctx);

//parse_commands.c
int	tokens_to_commands(t_token_list *tokens, t_cmd_list *commands);
int	add_arg_to_command(char *arg, t_cmd *cmd);
int	add_command_to_list(t_cmd *new_cmd, t_cmd_list *cmd_list);

//parser.c
int		parse_input(char *input, t_cmd_list	**commands);

//parsing_utils.c
void	add_to_buffer(char c, t_parser *parser);
int		init_tokenizer_ctx(t_tokenizer_ctx *ctx, char *input);
void	reset_buffer(t_parser *parser);
void	cleanup_tokenizer_ctx(t_tokenizer_ctx *ctx);

//state_machine.c
int handle_state_machine(char c, t_tokenizer_ctx *ctx);
int	handle_default_state(char c, t_tokenizer_ctx *ctx);
int	handle_double_quotes(char c, t_tokenizer_ctx *ctx);
int	handle_single_quotes(char c, t_tokenizer_ctx *ctx);

//token_utils.c
int		tokenize(char *input, t_tokenizer_ctx *ctx);
int 	safe_create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type);
int		create_and_add_token(t_tokenizer_ctx *ctx, t_token_type type);
void	add_token_list(t_token_list *token_list, t_token *token);
int		finalize_pending_token(t_tokenizer_ctx *ctx);

//var_state_handler.c
int	handle_variable_state(char c, t_tokenizer_ctx *ctx);
int	handle_first_var_char(char c, t_tokenizer_ctx *ctx);
int	handle_more_var_char(char c, t_tokenizer_ctx *ctx);

/****************EXECUTION**************** */

#endif