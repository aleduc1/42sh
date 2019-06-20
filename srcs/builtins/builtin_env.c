/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 17:36:44 by sbelondr          #+#    #+#             */
/*   Updated: 2019/05/28 11:18:19 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

static int		builtin_env_display(t_redirection *r)
{
	t_env	*lst;

	lst = get_env(3, NULL);
	if (!lst)
		ft_dprintf(r->error, "You are nothing value in env\n");
	while (lst && lst->next)
	{
		if (lst->see_env == 1)
			ft_dprintf(r->out, "%s=%s\n", lst->key, lst->value);
		lst = lst->next;
	}
	return (0);
}

static t_env	*builtin_env_key(t_env *my_env, char *argv, int *ret)
{
	if (ft_strchr_exist(argv, '='))
	{
		edit_set_command_env(argv, my_env);
		my_env = get_env(0, NULL);
	}
	else if (ft_strequ(argv, "-i"))
		my_env = get_env(1, NULL);
	else
		(*ret) = 1;
	return (my_env);
}

static int		builtin_env_s(t_redirection *r, char **argv, pid_t pid)
{
	t_env	*my_env;
	int		i;
	int		ret;

	i = 0;
	ret = 0;
	my_env = get_env(0, NULL);
	pid = fork();
	if (pid == 0)
	{
		while (argv[++i])
		{
			my_env = builtin_env_key(my_env, argv[i], &ret);
			if (ret == 1)
				break ;
		}
		if (!argv[i] || ft_strequ(argv[i], "env"))
			builtin_env_display(r);
		else
			ft_simple_command_redirection(argv + i, r);
		execve("/bin/test", NULL, NULL);
		exit(pid);
	}
	kill(pid, SIGINT);
	return (pid);
}

int				builtin_env(t_redirection *r, char **argv)
{
	pid_t	pid;
	int		rt;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = 0;
	if (!argv)
		builtin_env_display(r);
	else
		pid = builtin_env_s(r, argv, pid);
	while (wait(&rt) != -1)
		continue ;
	return (0);
}

int				builtin_set(t_redirection *r)
{
	int		i;
	char	**lst_env;

	lst_env = create_list_env(get_env(0, NULL), 0);
	i = -1;
	while (lst_env[++i])
		ft_dprintf(r->out, "%s\n", lst_env[i]);
	if (i == 0)
		ft_dprintf(r->error, "You are nothing value in env\n");
	ft_arraydel(&lst_env);
	return (0);
}
