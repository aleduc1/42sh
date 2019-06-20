/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_fc->c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 17:29:16 by mbellaic          #+#    #+#             */
/*   Updated: 2019/06/06 19:40:25 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh21.h"
#include "lexer.h"
#include "builtins.h"

#define FC_L (fc->flags[0] == 1)
#define FC_N (fc->flags[1] == 1)
#define FC_R (fc->flags[2] == 1)
#define FC_E (fc->flags[3] == 1)
#define FC_S (fc->flags[4] == 1)

typedef struct	s_fc
{
	char		*editor;
	int			flags[5];
	char		*first;
	char		*last;
	char		*pat_rep;
	char		*cmd;
	int			first_index;
	int			last_index;
}				t_fc;

void	init_fc(t_fc *fc)
{
	fc->editor = NULL;
	fc->first = NULL;
	fc->last = NULL;
	fc->first_index = 0;
	fc->last_index = 0;
	fc->pat_rep = NULL;
	fc->cmd = NULL;
}

int		fc_usage(int return_value, t_fc *fc, int error)
{
	if (error == 1)
		ft_dprintf(2, "fc: usage: fc -e [ename] [cmd]\n\t   fc [-l(nr)] [first] [last]\n\t   fc -s [pat=rep] [cmd]\n");
	if (error == 2)
		ft_dprintf(2, "21sh: fc: history specification our of range\n");
	ft_strdel(&fc->pat_rep);
	ft_strdel(&fc->first);
	ft_strdel(&fc->last);
	ft_strdel(&fc->editor);
	ft_strdel(&fc->cmd);
	return (return_value);
}

int		fc_flags_e(char **av, t_fc *fc)
{
	if (ft_strequ(av[1], "-e") == 1)
	{
		fc->flags[3] = 1;
		if (av[2])
			fc->editor = ft_strdup(av[2]);
		else
			return (fc_usage(-1, fc, 1));
		if (av[2] && av[3])
			fc->first = ft_strdup(av[3]);
		if (av[3] && av[4])
			fc->last = ft_strdup(av[4]);
		return (1);
	}
	return (0);
}

int		fc_flags_s(char **av, t_fc *fc)
{
	if (ft_strequ(av[1], "-s") == 1)
	{
		fc->flags[4] = 1;
		if (av[2])
			fc->pat_rep = ft_strdup(av[2]);
		else
			return (fc_usage(-1, fc, 1));
		if (av[2] && av[3])
			fc->first = ft_strdup(av[3]);
		// 	fc->cmd = ft_strdup(pos->history->next->line);
		return (1);
	}
	return (0);
}

int		fc_get_args(char **av, int argc, t_fc *fc)
{
	if (fc->flags[0] == 0 && (fc->flags[1] == 1 || fc->flags[2] == 1))
		return(fc_usage(-1, fc, 1));
	if (fc->flags[0] == 1)
	{
		if (av[argc])
			fc->first = ft_strdup(av[argc]);
		if (av[argc] && av[argc+1])
			fc->last = ft_strdup(av[argc+1]);
		return (1);
	}
	else
	{
		if (av[argc])
			fc->cmd = ft_strdup(av[argc]);
		return (1);
	}
	return (0);
}

int		fc_flags_lnr(char **av, t_fc *fc)
{
	int i;
	int j;

	i = 1;
	j = 1;
	while (av[i] && av[i][0] == '-' && av[i][1])
	{
		while (av[i][j])
		{
			if (av[i][j] == 'l')
				fc->flags[0] = 1;
			else if (av[i][j] == 'n')
				fc->flags[1] = 1;
			else if (av[i][j] == 'r')
				fc->flags[2] = 1;
			else
				return(fc_usage(-1, fc, 1));
			j++;
		}
		j = 1;
		i++;
	}
	return (fc_get_args(av, i, fc));
}

int		fc_flags(char **av, t_fc *fc)
{
	int ret;

	ret = 0;
	if ((ret = fc_flags_e(av, fc)))
		if (ret < 0 || ret > 0)
			return (ret);
	if ((ret = fc_flags_s(av, fc)))
		if (ret < 0 || ret > 0)
			return (ret);
	if ((ret = fc_flags_lnr(av, fc)))
		if (ret < 0 || ret > 0)
			return (ret);
	return (0);
}

void fc_debug(t_fc *fc)
{
	ft_dprintf(1, "l = %d || n = %d || r = %d || e = %d || s = %d\n",
	fc->flags[0], fc->flags[1], fc->flags[2], fc->flags[3], fc->flags[4]);
	if (fc->first)
		ft_dprintf(1, "first = %s\n", fc->first);
	if (fc->last)
		ft_dprintf(1, "last = %s\n", fc->last);
	if (fc->first_index)
		ft_dprintf(1, "first_index = %d\n", fc->first_index);
	if (fc->last_index)
		ft_dprintf(1, "last_index = %d\n", fc->last_index);
	if (fc->editor)
		ft_dprintf(1, "editor = %s\n", fc->editor);
	if (fc->cmd)
		ft_dprintf(1, "cmd = %s\n", fc->cmd);
	if (fc->pat_rep)
		ft_dprintf(1, "pat_rep = %s\n", fc->pat_rep);
}

int fc_count(t_node *history)
{
	t_node *lstcursor;
	int		count;

	count = 1;
	lstcursor = history;
	while (lstcursor->next)
		lstcursor = lstcursor->next;
	while (lstcursor->prev && lstcursor->prev->prev)
	{
		count++;
		lstcursor = lstcursor->prev;
	}
	return (count);
}

int fc_lr_basic(t_fc *fc, t_node *lstcursor, int count, int i)
{
	i = 1;
	if(lstcursor->next)
		lstcursor = lstcursor->next;
	while (lstcursor && i <= 15)
	{
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", count - i, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		i++;
		lstcursor = lstcursor->next;
	}
	return (1);
}

int fc_l_basic(t_fc *fc, t_node *lstcursor, int count, int i)
{
	if (FC_R)
		return (fc_lr_basic(fc, lstcursor, count, i));
	while (lstcursor->next && i++ < 14)
		lstcursor = lstcursor->next;
	while (lstcursor->prev && lstcursor->prev->prev)
	{
		if (!FC_N)
		{
			if (count <= 15)
				ft_dprintf(1, "%d	%s\n", count - (i) - 1, lstcursor->line);
			else
				ft_dprintf(1, "%d	%s\n", count - (i), lstcursor->line);
		}
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		i--;
		lstcursor = lstcursor->prev;
	}
	return (1);
}

int fc_lr_first_nb(t_fc *fc, t_node *lstcursor, int count, int i)
{
	while (lstcursor->next && count >= i)
	{
		lstcursor = lstcursor->next;
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", count--, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
	}
	return (1);
}

int fc_l_first_nb(t_fc *fc, t_node *lstcursor, int count, int i)
{
	t_node *backup_cursor;

	backup_cursor = lstcursor;
	i = 1;
	while (lstcursor->next)
		lstcursor = lstcursor->next;
	while (lstcursor->prev && i < ft_atoi(fc->first))
	{
		lstcursor = lstcursor->prev;
		if (!lstcursor->prev && i < ft_atoi(fc->first))
			return(fc_usage(-1, fc, 2));
		i++;
	}
	if (FC_R)
		return (fc_lr_first_nb(fc, backup_cursor, count - 1, i));
	while(lstcursor->prev && lstcursor->prev->prev)
	{
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", i++, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		lstcursor = lstcursor->prev;
	}
	return (1);
}

int fc_lr_first_word(t_fc *fc, t_node *lstcursor, int count, int i)
{
	if (lstcursor->next)
		lstcursor = lstcursor->next;
	while (lstcursor->next && i-- >= 0)
	{
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", count-- + 1, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		lstcursor = lstcursor->next;
	}
	return (1);
}

int fc_l_first_word(t_fc *fc, t_node *lstcursor, int count, int i)
{
	t_node *backup_cursor;
	int ret;

	i = -1;
	ret = 0;
	backup_cursor = lstcursor;
	while (lstcursor->next && \
	(ret = ft_strnequ(fc->first, lstcursor->line, ft_strlen(fc->first))) != 1)
	{
		i++;
		lstcursor = lstcursor->next;
	}
	if (ret != 1)
		return (fc_usage(-1, fc, 2));
	if (FC_R)
		return (fc_lr_first_word(fc, backup_cursor, count - 2, i));
	while (lstcursor->prev && lstcursor->prev->prev)
	{
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", count - (i--) - 1, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		lstcursor = lstcursor->prev;
	}
	return (1);
}

int	fc_l_first(t_fc *fc, t_node *lstcursor, int count, int i)
{
	int first_nb;

	first_nb = ft_isnumbers(fc->first);
	if (first_nb == 1 && (fc_l_first_nb(fc, lstcursor, count, i) == -1))
			return (-1);
	else if(first_nb != 1 && (fc_l_first_word(fc, lstcursor, count, i) == -1))
		return (-1);
	return (1);
}

int fc_get_index(t_node *lstcursor, int count, int *fc_index, char *fc_first_last)
{
	int ret;
	int i;

	i = 0;
	ret = 0;
	if (ft_isnumbers(fc_first_last) == 1)
	{
		*fc_index = ft_atoi(fc_first_last);
		if (*fc_index > count)
			return (-1);
		else
			return (1);
	}
	while (lstcursor->next && \
	(ret = ft_strnequ(fc_first_last, lstcursor->line, ft_strlen(fc_first_last))) != 1)
	{
		i++;
		lstcursor = lstcursor->next;
	}
	if (ret != 1)
		return (-1);
	*fc_index = count - i;
	return (1);
}

int fc_print_first_last(t_fc *fc, t_node *lstcursor, int i)
{
	i = 0;
	while (lstcursor->next)
		lstcursor = lstcursor->next;
	while (lstcursor->prev && lstcursor->prev->prev && ++i < fc->first_index)
		lstcursor = lstcursor->prev;
	while (lstcursor->prev && lstcursor->prev->prev && i <= fc->last_index)
	{
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", i, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		i++;
		lstcursor = lstcursor->prev;
	}
	return (1);
}

int fc_print_last_first(t_fc *fc, t_node *lstcursor, int i)
{
	i = 0;
	while (lstcursor->next)
		lstcursor = lstcursor->next;
	while (lstcursor->prev && ++i < fc->first_index)
		lstcursor = lstcursor->prev;
	while (lstcursor && i >= fc->last_index)
	{
		if (!FC_N)
			ft_dprintf(1, "%d	%s\n", i, lstcursor->line);
		else
			ft_dprintf(1, "%s\n", lstcursor->line);
		i--;
		lstcursor = lstcursor->next;
	}
	return (1);
}

int fc_l_first_last(t_fc *fc, t_node *lstcursor, int count, int i)
{
	if (fc_get_index(lstcursor, count, &fc->first_index, fc->first) == -1)
		return (fc_usage(-1, fc, 2));
	if (fc_get_index(lstcursor, count, &fc->last_index, fc->last) == -1)
		return (fc_usage(-1, fc, 2));
	if (fc->first_index <= fc->last_index && !FC_R)
		return (fc_print_first_last(fc, lstcursor, i));
	if (fc->first_index >= fc->last_index && !FC_R)
		return (fc_print_last_first(fc, lstcursor->next, i));
	if (fc->first_index <= fc->last_index && FC_R)
	{
		ft_swap(&fc->first_index, &fc->last_index);
		return (fc_print_last_first(fc, lstcursor, i));
	}
	if (fc->first_index >= fc->last_index && FC_R)
	{
		ft_swap(&fc->first_index, &fc->last_index);
		return (fc_print_first_last(fc, lstcursor, i));
	}
	return (1);
}

int fc_l(t_fc *fc, t_pos *pos)
{
	int count;
	int i;
	t_node *lstcursor;

	lstcursor = pos->history->next;
	count = fc_count(pos->history);
	i = -1;
	if (!fc->first)
		fc_l_basic(fc, lstcursor, count, i);
	else if (fc->first && !fc->last)
	{
		if (fc_l_first(fc, lstcursor, count, i) == -1)
			return (-1);
	}
	else if (fc->first && fc->last)
		if (fc_l_first_last(fc, lstcursor, count, i) == -1)
			return (-1);
	return (0);
}

int	fc_exec(t_fc *fc, t_pos *pos)
{
	if (FC_L)
	{
		if(fc_l(fc, pos) == -1)
			return (-1);
	}
	return (1);
}

int builtin_fc(char **av, t_pos *pos)
{
	t_fc fc;
	int i;

	i = 0;
	while (i < 5)
		fc.flags[i++] = 0;
	init_fc(&fc);
	if (fc_flags(av, &fc) == -1)
		return (-2);
	if (fc_exec(&fc, pos) == -1)
		return (-2);
	return 1;
}
