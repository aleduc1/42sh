/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apruvost <apruvost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 16:01:05 by apruvost          #+#    #+#             */
/*   Updated: 2019/08/20 05:36:10 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef BUILTINS_H
#	define BUILTINS_H

#	define HT_HASH_HASH_ONE 3
#	define HT_HASH_HASH_TWO 7
#	define HT_HASH_BASE_SIZE 50

#include "env.h"
#include "job.h"
#include <sys/param.h>

typedef struct		s_cd
{
	int		arg__;
	int		arg_l;
	int		arg_p;
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
	t_hash				*hash;
	struct s_hash_tree	*left;
	struct s_hash_tree	*right;

}					t_hash_tree;

typedef struct		s_repalias
{
	char				*key;
	char				*value;
	struct s_repalias	*next;
}					t_repalias;

int					bt_exit(t_job *j, t_pos *pos, t_redirection *r);
int					bt_echo(char **av, t_redirection *r);

int					bt_cd(char **av, t_redirection *r);
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

/*
** bt_jobs.c
*/

int					bt_jobs(t_job *j, char **av, t_redirection *r);

/*
** bt_bg.c
*/

int					bt_bg(t_job *j, char **av, t_redirection *r);

/*
** bt_fg.c
*/

int					bt_fg(t_job *j, char **av, t_redirection *r);

/*
** bt_fg_bg_tools.c
*/

t_job				*job_for_bg_fg(char **av, t_redirection *r);
void				change_fg_var_job(t_job *j, int fg);

/*
** bt_jobs_tools.c
*/

char				*assembly_cmd_process(t_process *p);
char				*cmd_job_s(t_job *j);
char				*ft_inter_signal(int sig, t_job *j);

/*
** bt_jobs_tools_bis.c
*/

char				*ft_name_sig(int sig);
t_job				*ft_search_exist_job(char *av, int index);

/*
** bt_jobs_options.c
*/

t_job				*search_job(char *av);
void				display_jobs_options(void (*p)(t_job*, int, t_redirection*),
						t_redirection *r, char **av);

/*
** bt_jobs_display.c
*/

char				value_char_job(int current, int max);
void				bt_jobs_p(t_job *j, int max_current, t_redirection *r);
void				bt_jobs_l(t_job *j, int max_current, t_redirection *r);
void				bt_jobs_s(t_job *j, int max_current, t_redirection *r);

/*
** search_job.c
*/

t_job				*last_jobs(void);
t_job				*previous_jobs(void);
t_job				*ft_search_pid_job(pid_t pid);
t_job				*ft_search_id_job(char *av, int index);
t_job				*ft_search_str_job(char *av, int index);

/*
** bt_export.c
*/
int					bt_export(char **value, t_redirection *r);

int					builtin_fc(char **av, t_pos *pos);

/*
** builtin_env.c
*/

int					builtin_env_display(t_redirection *r);

int					bt_alias(char **av, t_redirection *r);
int					bt_unalias(char **av, t_redirection *r);

void				hash_del(t_hash *hash);
t_hash				*hash_new_item(const char *k, const char *v);
t_ht_hash			*ht_hash_new(void);
t_ht_hash			*ht_hash_new_sized(const int base_size);
void				ht_hash_del(t_ht_hash *ht);
int					ht_hash_get_hash(const char *s, const int num,
						const int attempt);
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

int					bt_hash(char **av, t_redirection *r);

int					bt_type(char **av, t_redirection *r);

int					ft_getopt(int ac, char *const *av, const char *optstr);
void				ft_getopt_reset(void);

char				*alias_replace(char *input);

#	endif
