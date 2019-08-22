#include "builtins.h"

int		b_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -b");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFBLK)
			return (0);
	}
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary b");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		c_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -c");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFCHR)
			return (0);
	}
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary c");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		d_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -d");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFDIR)
			return (0);
	}
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary d");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		e_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -e");
	if (stat(str2, &sb) != -1)
			return (0);
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary e");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		f_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -f");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFREG)
			return (0);
	}
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary f");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		g_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -g");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary g");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		L_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -L");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary L");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		p_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -p");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary p");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		r_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -r");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary r");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		S_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -S");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary S");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		s_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -s");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary s");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		u_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -u");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary u");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		w_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -w");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary w");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		x_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -x");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary x");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}

int		z_test(char *str1, char *str2, char *str3)
{
<<<<<<< HEAD
	ft_putendl("je suis dans l'unary primary -z");
=======
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary z");
>>>>>>> a50095f9b10dbc063aee2677d57814a18f143a97
	return (1);
}
