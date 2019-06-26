/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 16:01:05 by sbelondr          #+#    #+#             */
/*   Updated: 2019/06/26 16:52:36 by apruvost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef BUILTINS_H
#	define BUILTINS_H

#include "env.h"
#include "job.h"
#include <sys/param.h>

typedef struct		s_cd
{
	int		arg__;
	int		arg_L;
	int		arg_P;
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

# define HT_HASH_HASH_ONE 3
# define HT_HASH_HASH_TWO 7
# define HT_HASH_BASE_SIZE 50

int					bt_exit(t_job *j);
int					bt_echo(char **av, t_redirection *r);

int					bt_cd(char **av);
int					cd_testcdpath(char *path, char *directory, char **test);
int					cd_testpath(char *path, char *directory, char **test);
int					cd_getnextpath(char *path);
int					cd_canonical(t_cd *cd);
int					cd_canonical_a(t_cd *cd);
int					cd_canonical_getdot(t_cd *cd, size_t *a, size_t *b);
int					cd_canonical_b(t_cd *cd);
int					cd_canonical_getprev(t_cd *cd, size_t *a, size_t *b);
size_t				cd_canonical_getprevv(t_cd *cd, size_t i);
int					cd_canonical_testprev(t_cd *cd, size_t *a);
int					cd_canonical_c(t_cd *cd);
int					cd_canonical_getslash(t_cd *cd, size_t *a, size_t *b);
int					cd_canonical_del(t_cd *cd, size_t a, size_t b, size_t len);
int					cd_chdir(t_cd *cd);
int					cd_err(t_cd *cd);
int					cd_getopt(char ac, char **av, t_cd *cd);

int					ft_getopt(int ac, char *const *av, const char *optstr);
void				ft_getopt_reset(void);

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

/*
** bt_jobs.c
*/

int					bt_jobs(char **av, t_redirection *r);
int					bt_bg(void);
int					bt_fg(void);
int					bt_export(char **value);

int					builtin_fc(char **av, t_pos *pos);

#	endif
