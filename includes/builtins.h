/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbellaic <mbellaic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 16:01:05 by sbelondr          #+#    #+#             */
/*   Updated: 2019/06/06 18:36:47 by mbellaic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef BUILTINS_H
#	define BUILTINS_H

#include "env.h"
#include "job.h"

typedef struct		s_cd
{
	int		arg__;
	char	*directory;
	char	*curpath;
}					t_cd;

typedef struct		s_hash
{
	char	*key;
	char	*value;
}					t_hash;

typedef struct		s_ht_hash
{
	int		size;
	int		base_size;
	int		count;
	t_hash	**hash;
}					t_ht_hash;

typedef struct		s_hash_tree
{
	t_hash	*hash;
	struct s_hash_tree	*left;
	struct s_hash_tree	*right;

}					t_hash_tree;

typedef struct		s_repalias
{
	char				*key;
	char				*value;
	struct s_repalias	*next;
}					t_repalias;

# define HT_HASH_HASH_ONE 3
# define HT_HASH_HASH_TWO 7
# define HT_HASH_BASE_SIZE 50

int					bt_exit(t_job *j);
int					bt_echo(char **av, t_redirection *r);

/*
** bt_jobs.c
*/

int					bt_jobs(char **av, t_redirection *r);
int					bt_bg(void);
int					bt_fg(void);
int					bt_export(char **value);

int					builtin_fc(char **av, t_pos *pos);

/*
** builtin_env.c
*/

int					builtin_env_display(t_redirection *r);





int					bt_alias(char **av);
int					bt_unalias(char **av);

void				hash_del(t_hash *hash);
t_hash				*hash_new_item(const char *k, const char *v);
t_ht_hash			*ht_hash_new(void);
t_ht_hash			*ht_hash_new_sized(const int base_size);
void				ht_hash_del(t_ht_hash *ht);
int					ht_hash_get_hash(const char *s, const int num, const int attempt);
int					ht_hash_hash(const char *s, const int a, const int m);
void				ht_hash_insert(t_ht_hash *ht, const char *key, char *val);
char				*ht_hash_search(t_ht_hash *ht, const char *key);
void				ht_hash_delete(t_ht_hash *ht, const char *key);
void				ht_hash_resize(t_ht_hash *ht, const int base_size);
void				ht_hash_copy(t_ht_hash *ht, t_ht_hash *new_ht);
void				ht_hash_resize_down(t_ht_hash *ht);
void				ht_hash_resize_up(t_ht_hash *ht);
void				ht_hash_table_null(t_hash **hash, int size);
void				ht_hash_show(t_ht_hash *ht, int quote);
void				ht_hash_reset(t_ht_hash **ht);

int					bt_hash(char **av);

int					bt_type(char **av);

int					ft_getopt(int ac, char *const *av, const char *optstr);
void				ft_getopt_reset(void);

char				*alias_replace(char *input);

#	endif
