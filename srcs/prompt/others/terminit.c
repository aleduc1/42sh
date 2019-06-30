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
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHO | ICANON | ISIG);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &term);
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
	interactive = isatty(STDIN_FILENO);
	if (interactive)
	{
		while (tcgetpgrp(STDIN_FILENO) != (pgid = getpgrp ()))
			kill (-pgid, SIGTTIN);
		ign_signaux();
		
		pgid = getpid();
		if (setpgid (pgid, pgid) < 0)
			exit(1);

		tcsetpgrp (STDIN_FILENO, getpid());
		tcgetattr(STDIN_FILENO, &term);

		term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG);
		term.c_cc[VMIN] = 1;
		term.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		tgetent(NULL, getenv("TERM"));
		s = get_shell();
		s->pgid = pgid;
		s->term = STDIN_FILENO;
		s->interactive = interactive;
		s->term_shell = term;
	}
}
