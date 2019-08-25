/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:01:09 by aleduc            #+#    #+#             */
/*   Updated: 2019/08/25 18:03:06 by mbellaic         ###   ########.fr       */
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
		if (ft_strcmp(argv[1], "--ast") == 0)
			g_print_ast = 1;
		if (ft_strcmp(argv[1], "-a") == 0)
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
	int		verif;

	lex = NULL;
	ast = NULL;
	input = history_replace(input, pos);
	input = alias_replace(input);
	verif = check_whitespace_input(input);
	if (verif && (lex = lexer(input)))
		if ((ast = ast_parser(lex)) && (solo_tree(ast, pos) < 0))
			interpreter(ast, pos, 0);
	if (input)
	{
		ft_strdel(&input);
		clean_lex(&lex);
		clean_ast(ast);
	}
}

static void	cpy_std(int in, int out, int error)
{
	char	*s_in;
	char	*s_out;
	char	*s_error;

	s_in = ft_itoa(in);
	s_out = ft_itoa(out);
	s_error = ft_itoa(error);
	add_set_value_perm("STDIN", s_in, 3);
	add_set_value_perm("STDOUT", s_out, 3);
	add_set_value_perm("STDERR", s_error, 3);
	ft_strdel(&s_in);
	ft_strdel(&s_out);
	ft_strdel(&s_error);
}

static void	ft_name_exec(char *name_exec)
{
	int		len;
	char	*cache;

	len = ft_strlen(name_exec);
	if (len < 3)
		return ;
	cache = ft_strsub(name_exec, 2, len);
	add_set_value("0", cache);
	add_set_value("EDITOR", "vim");
	add_set_value("FCEDIT", "vim");
	ft_strdel(&cache);
}

static void	edit_shell(char *name_exec)
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
	ft_name_exec(name_exec);
}

static void	init_alias(void)
{
	g_hash_table = ht_hash_new();
	g_alias_table = ht_hash_new();
	if (OS == 0)
		ht_hash_insert(g_alias_table, "ls", "ls -G");
	else
	{
		free_maillon_env("LS_COLORS", 0);
		ht_hash_insert(g_alias_table, "ls", "ls --color");
	}
	ht_hash_insert(g_alias_table, "b", "base64 /dev/urandom");
	ht_hash_insert(g_alias_table, "..", "cd ..");
	ht_hash_insert(g_alias_table, "-", "cd -");
	ht_hash_insert(g_alias_table, "?", "echo $?");
	ht_hash_insert(g_alias_table, "posix", "bash --posix");
}

int			main(int argc, char **argv, char **environ)
{
	t_multi	*multi_input;
	char	*input;
	t_pos	pos;

	input = NULL;
	multi_input = NULL;
	edit_shell(argv[0]);
	welcome();
	flags(argc, argv);
	init_prompt(&pos);
	init_alias();
	while (21)
	{
		if (argc && argv && environ)
			if ((input = prompt(multi_input, &pos)))
				run(input, &pos);
		job_notif();
		manage_id_job(0);
	}
	return (0);
}
