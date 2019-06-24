/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 17:19:28 by aleduc            #+#    #+#             */
/*   Updated: 2019/06/01 19:56:54 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "job.h"
#include "builtins.h"

void				welcome(void)
{
	ft_putstr("\033[2J\033[H");
	ft_putstr("\033[1;32m\n");
	ft_putstr("██╗  ██╗██████╗ ███████╗██╗  ██╗    \n");
	ft_putstr("██║  ██║╚════██╗██╔════╝██║  ██║    \n");
	ft_putstr("███████║ █████╔╝███████╗███████║    \n");
	ft_putstr("╚════██║██╔═══╝ ╚════██║██╔══██║    \n");
	ft_putstr("     ██║███████╗███████║██║  ██║    \n");
	ft_putstr("     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝    \n");
	ft_putendl("\033[0m");
}

void				default_term_mode(void)
{
	struct termios	term;

	// g_in = STDIN_FILENO;//open(ttyname(STDIN_FILENO), O_WRONLY);
	dup2(STDIN_FILENO, g_in);
	tcgetattr(g_in, &term);
	term.c_lflag |= (ECHO | ICANON | ISIG);
	tcsetattr(g_in, TCSADRAIN, &term);
	ft_putstr(tgetstr("ei", NULL));
}

void				raw_term_mode(void)
{
	// struct termios	term;
	// t_shell			*s;

	// s = get_shell();
	// s->term = STDIN_FILENO;
	// while (tcgetpgrp (s->term) != (s->pgid = getpgrp()))
	// 	kill (- s->pgid, SIGTTIN);
	// ign_signaux();
	// tcgetattr(STDIN_FILENO, &term);
	// term.c_lflag &= ~(ICANON | ECHO | ISIG);
	// term.c_cc[VMIN] = 1;
	// term.c_cc[VTIME] = 0;
	// tcsetattr(STDIN_FILENO, TCSANOW, &term);
	// tgetent(NULL, getenv("TERM"));
	// s->pgid = getpid();
	// s->interactive = isatty(s->term);
	// s->term_shell = term;




	struct termios	term;
	t_shell			*s;
	int				pgid;
	int				interactive;

	// g_in = STDIN_FILENO;//open(ttyname(STDIN_FILENO), O_WRONLY);
	interactive = isatty(g_in);
	if (interactive)
	{
		while (tcgetpgrp(g_in) != (pgid = getpgrp ()))
			kill (-pgid, SIGTTIN);
		ign_signaux();
		
		pgid = getpid();
		if (setpgid (pgid, pgid) < 0)
			exit(1);

		tcsetpgrp (g_in, getpid());
		tcgetattr(g_in, &term);

		term.c_lflag &= ~(ECHO | ECHONL | ICANON);
		term.c_cc[VMIN] = 1;
		term.c_cc[VTIME] = 0;
		tcsetattr(g_in, TCSANOW, &term);
		tgetent(NULL, getenv("TERM"));
		s = get_shell();
		s->pgid = pgid;
		s->term = g_in;
		s->interactive = interactive;
		s->term_shell = term;
	}
}
