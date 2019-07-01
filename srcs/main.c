/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:01:09 by aleduc            #+#    #+#             */
/*   Updated: 2019/06/30 19:40:36 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "lexer.h"
#include "parser.h"
#include "env.h"
#include "job.h"

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
	if (isatty(STDIN_FILENO) == 0)
		return (1);
	g_in = open(ttyname(STDIN_FILENO), O_WRONLY);
	if ((dup2(STDIN_FILENO, g_in)) == -1)
		exit(1);
	int out = open(ttyname(STDOUT_FILENO), O_WRONLY);
	if ((dup2(STDOUT_FILENO, out)) == -1)
		exit(1);
	// ign_signaux();
	(argc == 1) ? welcome() : 0;
	flags(argc, argv);
	init_prompt(&pos);
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
