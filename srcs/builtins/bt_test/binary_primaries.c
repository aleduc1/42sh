#include "builtins.h"

int		equal_test(char *str1, char *str2, char *str3)
{
	ft_putendl("Je suis dans le binary primary =");
	if (ft_strcmp(str1, str3) == 0)
		return (0);
	return (1);
}

int		notequal_test(char *str1, char *str2, char *str3)
{
	ft_putendl("Je suis dans le binary primary !=");
	if (ft_strcmp(str1, str3) == 0)
		return (1);
	return (0);
}

int		eq_test(char *str1, char *str2, char *str3)
{
	int		int1;
	int		int3;

	ft_putendl("Je suis dans le binary primary -eq");
	int1 = atoi(str1);
	int3 = atoi (str3);
	if (int1 == int3)
		return (0);
	return (1);
}

int		ne_test(char *str1, char *str2, char *str3)
{
	int		int1;
	int		int3;

	ft_putendl("Je suis dans le binary primary -ne");
	int1 = atoi(str1);
	int3 = atoi (str3);
	if (int1 == int3)
		return (1);
	return (0);
}

int		ge_test(char *str1, char *str2, char *str3)
{
	int		int1;
	int		int3;

	ft_putendl("Je suis dans le binary primary -ge");
	int1 = atoi(str1);
	int3 = atoi (str3);
	if (int1 >= int3)
		return (0);
	return (1);
}

int		lt_test(char *str1, char *str2, char *str3)
{
	int		int1;
	int		int3;

	ft_putendl("Je suis dans le binary primary -lt");
	int1 = atoi(str1);
	int3 = atoi (str3);
	if (int1 < int3)
		return (0);
	return (1);
}

int		le_test(char *str1, char *str2, char *str3)
{
	int		int1;
	int		int3;

	ft_putendl("Je suis dans le binary primary -le");
	int1 = atoi(str1);
	int3 = atoi (str3);
	if (int1 <= int3)
		return (0);
	return (1);
}
