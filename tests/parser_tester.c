/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tester.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwannhed <cwannhed@student.42firenze.it>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 15:00:00 by cwannhed          #+#    #+#             */
/*   Updated: 2025/08/27 16:28:02 by cwannhed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h>

// Colori per output più leggibile
#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

// Struttura per i test
typedef struct s_test_case
{
	char *name;
	char *input;
	char **expected_args;
	char *expected_output_file;
	char *expected_input_file;
	char *expected_heredoc_delimiter;
	int expected_append_mode;
	int expected_is_heredoc;
	int expected_cmd_count;
	int should_fail;
}	t_test_case;

// Categorie di test
typedef struct s_test_suite
{
	char *category_name;
	t_test_case *tests;
	int count;
}	t_test_suite;

// ===================== TEST CASES =====================

// Test basici
t_test_case basic_tests[] = {
	{"Simple command", "echo hello", (char *[]){"echo", "hello", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Command with multiple args", "ls -la /tmp", (char *[]){"ls", "-la", "/tmp", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Empty input", "", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{"Only spaces", "   ", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{"Command with trailing spaces", "echo hello   ", (char *[]){"echo", "hello", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0}
};

// Test per quotes
t_test_case quote_tests[] = {
	{"Single quotes", "echo 'hello world'", (char *[]){"echo", "hello world", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Double quotes", "echo \"hello world\"", (char *[]){"echo", "hello world", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Mixed quotes", "echo 'hello' \"world\"", (char *[]){"echo", "hello", "world", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Quotes with spaces", "echo \"hello   world\"", (char *[]){"echo", "hello   world", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Empty quotes", "echo \"\"", (char *[]){"echo", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Backslash literal", "echo \"\\\"", (char *[]){"echo", "\\", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Unclosed double quote", "echo \"hello", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{"Unclosed single quote", "echo 'hello", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0}
};

// Test per variabili
t_test_case variable_tests[] = {
	{"Simple variable", "echo $USER", (char *[]){"echo", "testuser", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Variable concatenation", "echo hello$USER", (char *[]){"echo", "hellotestuser", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Multiple variables", "echo $USER $HOME", (char *[]){"echo", "testuser", "/home/testuser", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Variable in quotes", "echo \"Hello $USER\"", (char *[]){"echo", "Hello testuser", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Variable in single quotes", "echo '$USER'", (char *[]){"echo", "$USER", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Exit status variable", "echo $?", (char *[]){"echo", "0", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Undefined variable", "echo $UNDEFINED", (char *[]){"echo", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0}
};

// Test per redirezioni
t_test_case redirect_tests[] = {
	{"Output redirect", "echo test > file.txt", (char *[]){"echo", "test", NULL}, "file.txt", NULL, NULL, 0, 0, 1, 0},
	{"Input redirect", "cat < input.txt", (char *[]){"cat", NULL}, NULL, "input.txt", NULL, 0, 0, 1, 0},
	{"Append redirect", "echo test >> file.txt", (char *[]){"echo", "test", NULL}, "file.txt", NULL, NULL, 1, 0, 1, 0},
	{"Heredoc", "cat << EOF", (char *[]){"cat", NULL}, NULL, NULL, "EOF", 0, 1, 1, 0},
	{"Multiple redirections", "cat < input.txt > output.txt", (char *[]){"cat", NULL}, "output.txt", "input.txt", NULL, 0, 0, 1, 0},
	{"Redirect without file", "echo >", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{"Redirect with spaces", "echo test >   file.txt", (char *[]){"echo", "test", NULL}, "file.txt", NULL, NULL, 0, 0, 1, 0},
	{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0}
};

// Test per pipes
t_test_case pipe_tests[] = {
	{"Simple pipe", "echo hello | cat", NULL, NULL, NULL, NULL, 0, 0, 2, 0},
	{"Multiple pipes", "cat file | grep test | sort", NULL, NULL, NULL, NULL, 0, 0, 3, 0},
	{"Pipe with redirections", "echo hello | cat > output.txt", NULL, NULL, NULL, NULL, 0, 0, 2, 0},
	{"Pipe at start", "| echo hello", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{"Pipe at end", "echo hello |", NULL, NULL, NULL, NULL, 0, 0, 0, 1},
	{"Pipe with spaces", "echo hello   |   cat", NULL, NULL, NULL, NULL, 0, 0, 2, 0},
	{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0}
};

// Test complessi
t_test_case complex_tests[] = {
	{"Quote merge", "echo \"hello\"world", (char *[]){"echo", "helloworld", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Variable and quote merge", "echo $USER\"test\"", (char *[]){"echo", "testusertest", NULL}, NULL, NULL, NULL, 0, 0, 1, 0},
	{"Complex command", "echo \"Hello $USER\" > /tmp/output.txt", (char *[]){"echo", "Hello testuser", NULL}, "/tmp/output.txt", NULL, NULL, 0, 0, 1, 0},
	{"Pipe with variables", "echo $USER | grep test", NULL, NULL, NULL, NULL, 0, 0, 2, 0},
	{"Mixed quotes and redirections", "echo 'test' \"$USER\" > file.txt", (char *[]){"echo", "test", "testuser", NULL}, "file.txt", NULL, NULL, 0, 0, 1, 0},
	{NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0}
};

// Suite di test
t_test_suite test_suites[] = {
	{"Basic Commands", basic_tests, 5},
	{"Quote Handling", quote_tests, 7},
	{"Variable Expansion", variable_tests, 7},
	{"Redirections", redirect_tests, 7},
	{"Pipes", pipe_tests, 6},
	{"Complex Cases", complex_tests, 5},
	{NULL, NULL, 0}
};

// ===================== UTILITY FUNCTIONS =====================

void print_header(const char *title)
{
	int len = strlen(title);
	int padding = (60 - len) / 2;
	
	printf("\n" BOLD CYAN "═");
	for (int i = 0; i < 58; i++)
		printf("═");
	printf("═\n" RESET);
	
	printf(BOLD CYAN "║" RESET);
	for (int i = 0; i < padding; i++)
		printf(" ");
	printf(BOLD WHITE "%s" RESET, title);
	for (int i = 0; i < (59 - len - padding); i++)
		printf(" ");
	printf(BOLD CYAN "║\n" RESET);
	
	printf(BOLD CYAN "═");
	for (int i = 0; i < 58; i++)
		printf("═");
	printf("═\n" RESET);
}

void print_test_result(const char *test_name, int passed, const char *error_msg)
{
	printf("%-40s ", test_name);
	if (passed)
		printf("[" BOLD GREEN " PASS " RESET "]");
	else
	{
		printf("[" BOLD RED " FAIL " RESET "]");
		if (error_msg)
			printf(" " YELLOW "(%s)" RESET, error_msg);
	}
	printf("\n");
}

void print_summary(int total, int passed, int failed)
{
	printf("\n" BOLD WHITE "═══════════════════════════════════════════════════════════\n" RESET);
	printf(BOLD WHITE "SUMMARY: " RESET);
	printf(BOLD GREEN "%d passed" RESET ", ", passed);
	printf(BOLD RED "%d failed" RESET ", ", failed);
	printf(BOLD WHITE "%d total\n" RESET, total);
	
	double percentage = total > 0 ? (double)passed / total * 100 : 0;
	printf(BOLD WHITE "Success rate: " RESET);
	if (percentage >= 80)
		printf(BOLD GREEN "%.1f%%\n" RESET, percentage);
	else if (percentage >= 60)
		printf(BOLD YELLOW "%.1f%%\n" RESET, percentage);
	else
		printf(BOLD RED "%.1f%%\n" RESET, percentage);
	
	printf(BOLD WHITE "═══════════════════════════════════════════════════════════\n" RESET);
}

// ===================== TEST FUNCTIONS =====================

int compare_string_arrays(char **arr1, char **arr2)
{
	if (!arr1 && !arr2)
		return 1;
	if (!arr1 || !arr2)
		return 0;
	
	int i = 0;
	while (arr1[i] && arr2[i])
	{
		if (strcmp(arr1[i], arr2[i]) != 0)
			return 0;
		i++;
	}
	return (arr1[i] == NULL && arr2[i] == NULL);
}

int validate_command_count(t_cmd_list *cmd_list, int expected_count)
{
	if (!cmd_list)
		return expected_count == 0;
	return cmd_list->count == expected_count;
}

int validate_single_command(t_cmd *cmd, t_test_case *test)
{
	// Verifica argomenti
	if (test->expected_args && !compare_string_arrays(cmd->args, test->expected_args))
		return 0;
	
	// Verifica output file
	if (test->expected_output_file)
	{
		if (!cmd->output_file || strcmp(cmd->output_file, test->expected_output_file) != 0)
			return 0;
		if (cmd->append_mode != test->expected_append_mode)
			return 0;
	}
	
	// Verifica input file
	if (test->expected_input_file)
	{
		if (!cmd->input_file || strcmp(cmd->input_file, test->expected_input_file) != 0)
			return 0;
	}
	
	// Verifica heredoc
	if (test->expected_is_heredoc)
	{
		if (!cmd->is_heredoc)
			return 0;
		if (test->expected_heredoc_delimiter && 
			(!cmd->heredoc_delimiter || strcmp(cmd->heredoc_delimiter, test->expected_heredoc_delimiter) != 0))
			return 0;
	}
	
	return 1;
}

int run_single_test(t_test_case *test, t_env *env)
{
	t_cmd_list *cmd_list = NULL;
	
	// Redirect stdout per nascondere i print di debug
	fflush(stdout);
	int stdout_fd = dup(STDOUT_FILENO);
	int devnull = open("/dev/null", O_WRONLY);
	dup2(devnull, STDOUT_FILENO);
	close(devnull);
	
	int result = parse_input(test->input, &cmd_list, &env);
	
	// Ripristina stdout
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdout_fd);
	
	// Test che dovrebbero fallire
	if (test->should_fail)
	{
		if (result == -1)
		{
			print_test_result(test->name, 1, NULL);
			return 1;
		}
		else
		{
			print_test_result(test->name, 0, "should have failed");
			if (cmd_list)
				free_command_list(cmd_list);
			return 0;
		}
	}
	
	// Test che dovrebbero avere successo
	if (result == -1)
	{
		print_test_result(test->name, 0, "parsing failed");
		return 0;
	}
	
	// Verifica numero comandi
	if (!validate_command_count(cmd_list, test->expected_cmd_count))
	{
		print_test_result(test->name, 0, "wrong command count");
		if (cmd_list)
			free_command_list(cmd_list);
		return 0;
	}
	
	// Se ci aspettiamo 0 comandi, il test è già passato
	if (test->expected_cmd_count == 0)
	{
		print_test_result(test->name, 1, NULL);
		if (cmd_list)
			free_command_list(cmd_list);
		return 1;
	}
	
	// Verifica primo comando (per i test con cmd_count > 1, verifichiamo solo che siano stati creati)
	if (cmd_list && cmd_list->head && test->expected_cmd_count == 1)
	{
		if (!validate_single_command(cmd_list->head, test))
		{
			print_test_result(test->name, 0, "command validation failed");
			free_command_list(cmd_list);
			return 0;
		}
	}
	
	print_test_result(test->name, 1, NULL);
	if (cmd_list)
		free_command_list(cmd_list);
	return 1;
}

int run_test_suite(t_test_suite *suite, t_env *env)
{
	printf("\n" BOLD MAGENTA "┌─ %s " RESET, suite->category_name);
	for (int i = strlen(suite->category_name) + 3; i < 58; i++)
		printf("─");
	printf(BOLD MAGENTA "┐\n" RESET);
	
	int passed = 0;
	int total = 0;
	
	for (int i = 0; suite->tests[i].name; i++)
	{
		passed += run_single_test(&suite->tests[i], env);
		total++;
	}
	
	printf(BOLD MAGENTA "└" RESET);
	for (int i = 0; i < 58; i++)
		printf("─");
	printf(BOLD MAGENTA "┘\n" RESET);
	
	printf(BOLD WHITE "Suite result: " RESET "%d/%d tests passed\n", passed, total);
	
	return passed;
}

// ===================== MAIN FUNCTION =====================

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	
	// Setup ambiente di test
	setenv("USER", "testuser", 1);
	setenv("HOME", "/home/testuser", 1);
	
	t_env *env = env_to_list(envp);
	if (!env)
	{
		printf(BOLD RED "Error: Could not initialize environment\n" RESET);
		return 1;
	}
	
	print_header("MINISHELL PARSER TESTER");
	
	int total_passed = 0;
	int total_tests = 0;
	
	// Esegui tutte le suite di test
	for (int i = 0; test_suites[i].category_name; i++)
	{
		int suite_passed = run_test_suite(&test_suites[i], env);
		total_passed += suite_passed;
		
		// Conta il totale dei test in questa suite
		for (int j = 0; test_suites[i].tests[j].name; j++)
			total_tests++;
	}
	
	// Stampa riepilogo finale
	print_summary(total_tests, total_passed, total_tests - total_passed);
	
	free_env(env);
	return (total_passed == total_tests ? 0 : 1);
}