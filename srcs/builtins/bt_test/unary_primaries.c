#include "builtins.h"

/*
** I need to do Read, Write, and Execute options (-r, -w, -x)
** Except that everything else should work, but need to test each options
*/

int		b_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -b");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFBLK)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary b");
	return (1);
}

int		c_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -c");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFCHR)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary c");
	return (1);
}

int		d_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -d");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFDIR)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary d");
	return (1);
}

int		e_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -e");
	if (stat(str2, &sb) != -1)
			return (0);
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary e");
	return (1);
}

int		f_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -f");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFREG)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary f");
	return (1);
}

int		g_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -g");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_ISGID)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		L_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -L");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFLNK)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		p_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -p");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFIFO)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		r_test(char *str1, char *str2, char *str3)
{
	ft_putendl("je suis dans l'unary primary -r");
	(void)str1;(void)str2;(void)str3;
	ft_putendl("je suis dans l'unary primary r");
	return (1);
}

int		S_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -S");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_IFIFO)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		s_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -s");
	if (stat(str2, &sb) != -1)
	{
		if (sb.st_size > 0)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		u_test(char *str1, char *str2, char *str3)
{
	struct stat	sb;

	ft_putendl("je suis dans l'unary primary -u");
	if (stat(str2, &sb) != -1)
	{
		if ((sb.st_mode & S_IFMT) == S_ISUID)
			return (0);
	}
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		w_test(char *str1, char *str2, char *str3)
{
	ft_putendl("je suis dans l'unary primary -w");
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		x_test(char *str1, char *str2, char *str3)
{
	ft_putendl("je suis dans l'unary primary -x");
	(void)str1;(void)str2;(void)str3;
	return (1);
}

int		z_test(char *str1, char *str2, char *str3)
{
	ft_putendl("je suis dans l'unary primary -z");
	if (ft_strlen(str2) == 0)
		return (0);
	(void)str1;(void)str2;(void)str3;
	return (1);
}
