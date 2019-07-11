/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 13:33:53 by sbelondr          #+#    #+#             */
/*   Updated: 2019/07/08 00:11:36 by sbelondr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#	ifndef ENV_H
#	define ENV_H

#include "sh21.h"
#include "parser.h"
#include "libft.h"
#include <curses.h>
#include <term.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct		s_redirect
{
	int					base;
	int					new_fd;
	struct s_redirect	*next;
}					t_redirect;

typedef struct		s_redirection
{
	int			in;
	int			out;
	int			error;
	t_redirect	*redirect;
}					t_redirection;

typedef struct		s_env
{
	char			*key;
	char			*value;
	int				see_env;
	struct s_env	*next;
}					t_env;

typedef struct		s_shell
{
	pid_t			pgid;
	struct termios	term_shell;
	int				interactive;
	int				term;
}					t_shell;

typedef struct		s_process
{
	char				*cmd_path;
	char				**cmd;
	int					process_id;
	pid_t				pid;
	int					completed;
	int					stopped;
	int					status;
	int					return_value;
	int					fg;
	t_redirection		*r;
	struct s_process	*next;
}					t_process;

typedef struct		s_job
{
	t_process		*first_process;
	pid_t			pgid;
	char			*cmd;
	int				fg;
	int				notified;
	int				notif_stop;
	struct termios	tmodes;
	int				len_close;
	int				*close_fd;
	t_redirection	*r;
	struct s_job	*next;
}					t_job;

void				parser_var_test(char **value);

int					gest_error_path(char *cmd, t_redirection *r);

/*
** redirection.c
*/

void				redirection_fd(t_redirection *r);

/*
** rediretion_bis.c
*/

void				redir_in(t_redirection *r);
void				redir_out(t_redirection *r);
void				redir_error(t_redirection *r);
void				other_redir(int src, int new_fd);

/*
** check_last.c
*/

int					check_last_command(void);
int					check_is_exec(char *src, t_redirection *r);

/*
** manage_variable.c
*/

char				*manage_var(char *str);
int					manage_home(char *src, char **dst, int index);
int					modify_dst(char *tmp, char **dst);

/*
** list_redirection.c
*/

t_redirection		*init_redirection(void);
t_redirection		*fill_redirection(t_token *t);
t_redirection		*base_redirection(void);

/*
** delete_redirection.c
*/

void				delete_redirection(t_redirection **r);
int					verif_close(int fd);

/*
** other_tools.c
*/

int					gest_return(int verif);
int					is_builtin(t_job *j, t_process *p, t_pos *pos);
char				*is_in_path(char *command);
t_env				*get_env(int is_end, t_env *head);

/*
** parser_var.c
*/

void				parser_var(char ***value);
char				*search_var(char *src);
void				parser_var_simple(char **value);

/*
** apply_parser_var.c
*/

int					apply_rules(char *src, char **dst, int index);

/*
** manage_env.c
*/

int					edit_setenv(char *key, char *value);
int					edit_export(char *key);
int					ft_unsetenv(char *key);
char				**create_list_env(t_env *my_env, int env);

/*
** manage_set.c
*/

int					edit_set(char **value, t_redirection *r);
int					add_set_value(char *key, char *value);
int					ft_unset(char *key);
int					edit_set_command_env(char *str, t_env *my_env);

/*
** tools_env.c
*/

char				*value_line_path(char *key, int env);
int					create_new_line_env(t_env *my_env, char *key, char *value,
		int env);

/*
** builtin_env.c
*/

int					builtin_set(t_redirection *r);
int					builtin_env(t_redirection *r, char **argv);

/*
** builtin_cd.c
*/

int					builtin_cd(char **arguments);

/*
** builtin_cd_verif.c
*/

int					ft_lastchr(const char *s, int c);
void				check_arg(char **str);
void				error_cd(int code, char *str);

/*
** list_env.c
*/

t_env				*init_env(void);
t_env				*init_maillon_env(void);
void				init_variable(void);
t_env				*ft_cpy_env(void);

/*
** free_env.c
*/

void				free_env(t_env **env);
int					free_maillon_env(char *key, int env);

/*
** manage_file.c
*/

int					open_file_command(t_redir *redir, t_pos *pos);
int					close_file_command(t_lex *lex, t_redirection **r);
int					file_exist(char *name, int type);

/*
** commands.c
*/

void				display_error_command(t_redirection *r, char **cmd);
int					ft_simple_command(char **argv, t_token *lex, t_pos *pos);
int					ft_simple_command_redirection(char **argv,
						t_redirection *r);
int					ft_pipe_double(char **argv, t_token *token);
int					ft_ampersand(char **argv, t_token *token);
int					ft_ampersand_double(char **argv, t_token *token);

/*
** commands_pipe.c
*/

int					ft_pipe(char **argv, t_token *lex, int end_pipe);
int					add_pipe_process(char **cmd, t_redirection *r);

/*
** manage_quote.c
*/

void				ft_remove_quote(char **str);
int					ft_apply_dquote(char ***value, int index);

/*
** main.c
*/

void				run(char *input, t_pos *pos);

/*
** parameter_expansion.c
*/

void				parameter_expansion(char *tmp, char **dst);

/*
** formats_parameter_bis.c
*/

char				*parameter_moins(char *parameter, char *word);
char				*parameter_equals(char *parameter, char *word);
char				*parameter_interrogation(char *parameter, char *word);
char				*parameter_plus(char *parameter, char *word);

/*
** formats_parameter.c
*/

char				*parameter_hash_first(char *parameter);
char				*parameter_hash_end(char *parameter);
char				*parameter_percents(char *parameter);

/*
** list_redirect.c
*/

int					ft_create_maillon_redirect(t_redirect *r, int base,
						int new_fd);
t_redirect			*ft_init_redirect(void);
int					ft_fd_redirect_exist(t_redirect *r, int base);

/*
** add_process.c
*/

t_job				*get_end_job(void);
void				add_process(char **av, t_token *t, int fg);

/*
**  debug
*/

void				display_redirection(t_redirection *r);

/*
** test_quote.c
*/

int					test_quote(char *name);

#	endif
