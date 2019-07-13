/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:01:09 by aleduc            #+#    #+#             */
/*   Updated: 2019/07/12 02:48:21 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "lexer.h"
#include "parser.h"
#include "env.h"
#include "job.h"
#include "builtins.h"

t_ht_hash	*g_hash_table;
t_ht_hash	*g_alias_table;

int			siginthandler(int signum)
{
	(void)signum;
	ft_printf("signal handler\n");
	return (0);
}

void		flags(int argc, char **argv)
{
	g_print_ast = 0;
	if (argc >= 2)
	{
		if (strcmp(argv[1], "--ast") == 0)
			g_print_ast = 1;
		if (strcmp(argv[1], "-a") == 0)
			g_print_ast = 1;
	}
	return ;
}

int			check_whitespace_input(char *input)
{
	int		i;

	i = 0;
	while (input && input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else
			return (1);
	}
	return (0);
}

void		run(char *input, t_pos *pos)
{
	t_lex	*lex;
	t_ast	*ast;

	lex = NULL;
	ast = NULL;
	input = alias_replace(input);
	if ((check_whitespace_input(input)) && (lex = lexer(input)))
	{
		ft_strdel(&input);
		if ((ast = ast_parser(lex)) && (solo_tree(ast, pos) < 0))
			interpreter(ast, pos);
		clean_lex(&lex);
		clean_ast(ast);
	}
	else if (input)
	{
		ft_strdel(&input);
		clean_lex(&lex);
		clean_ast(ast);
	}
}

void		script_test(char **av, t_pos pos)
{
	int		i;
	char	*str;

	i = -1;
	while (av[++i])
	{
		str = ft_strdup(av[i]);
		run(str, &pos);
	}
	exit(0);
}

static void	cpy_std(int in, int out, int error)
{
	char	*s_in;
	char	*s_out;
	char	*s_error;

	s_in = ft_itoa(in);
	s_out = ft_itoa(out);
	s_error = ft_itoa(error);
	add_set_value("STDIN", s_in);
	add_set_value("STDOUT", s_out);
	add_set_value("STDERR", s_error);
	ft_strdel(&s_in);
	ft_strdel(&s_out);
	ft_strdel(&s_error);
}

static void	edit_shell(void)
{
	int	in;
	int	out;
	int	error;

	if (isatty(STDIN_FILENO) == 0 || isatty(STDOUT_FILENO) == 0
		|| isatty(STDERR_FILENO) == 0)
		exit(1);
	in = open(ttyname(STDIN_FILENO), O_WRONLY);
	out = open(ttyname(STDOUT_FILENO), O_WRONLY);
	error = open(ttyname(STDERR_FILENO), O_WRONLY);
	if ((dup2(STDIN_FILENO, in)) == -1)
		exit(1);
	if ((dup2(STDOUT_FILENO, out)) == -1)
		exit(1);
	if ((dup2(STDERR_FILENO, out)) == -1)
		exit(1);
	cpy_std(in, out, error);
}

static void	init_alias(void)
{
	g_hash_table = ht_hash_new();
	g_alias_table = ht_hash_new();
	ht_hash_insert(g_alias_table, "ls", "ls -G");
	ht_hash_insert(g_alias_table, "..", "cd ..");
	ht_hash_insert(g_alias_table, "-", "cd -");
	ht_hash_insert(g_alias_table, "?", "echo $?");
}

int			main(int argc, char **argv, char **environ)
{
	t_multi	*multi_input;
	char	*input;
	t_pos	pos;

	input = NULL;
	multi_input = NULL;
	(argc == 1) ? edit_shell() : 0;
	(argc == 1) ? welcome() : 0;
	flags(argc, argv);
	init_prompt(&pos);
	init_alias();
	if (argc > 1)
		script_test(argv + 1, pos);
	while (21)
	{
		if (argc && argv && environ)
			if ((input = prompt(multi_input, &pos)))
			{
				run(input, &pos);
				job_notif();
			}
	}
	return (0);
}
