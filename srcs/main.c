/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:01:09 by aleduc            #+#    #+#             */
/*   Updated: 2019/07/03 18:36:32 by apruvost         ###   ########.fr       */
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

int		siginthandler(int signum)
{
	(void)signum;
	ft_printf("signal handler\n");
	return (0);
}

void	flags(int argc, char **argv)
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

int		check_whitespace_input(char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else
			return (1);
	}
	return (0);
}

void	run(char *input, t_pos *pos)
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

void	script_test(char **av, t_pos pos)
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

int		main(int argc, char **argv, char **environ)
{
	t_multi	*multi_input;
	char	*input;
	t_pos	pos;

	input = NULL;
	multi_input = NULL;
//	ign_signaux();
	(argc == 1) ? welcome() : 0;
	flags(argc, argv);
	init_prompt(&pos);
	g_hash_table = ht_hash_new();
	g_alias_table = ht_hash_new();
	if (argc > 1)
		script_test(argv + 1, pos);
	while (21)
	{
		if (argc && argv && environ)
			if ((input = prompt(multi_input, &pos)))
				run(input, &pos);
	}
	return (0);
}
