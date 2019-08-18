#include "builtins.h"

int		bt_1(char *av1)
{
	if (av1[0])
		return (0);
	return (1);
}

int		bt_2(char *av1, char *av2)
{
	int ret;

	ret = 0;
	if (ft_strcmp(av1, "!") == 0)
	{
		ret = (bt_1(av2));
		if (ret == 1)
			return (0);
		else
			return (1);
	}
	if (is_unary(av1))
		return (unary_test(av1, av2));
	return (1);
}

int		bt_3(char *av1, char *av2, char *av3)
{
	int		ret;

	ret = 0;
	if (is_binary(av2))
		return (binary_test(av1, av2, av3));
	if (ft_strcmp(av1, "!") == 0)
	{
		ret = (bt_2(av2, av3));
		if (ret == 1)
			return (0);
		else
			return (1);
	}
	if (ft_strcmp(av1, "(") == 0 && ft_strcmp(av3, ")") == 0)
		return (bt_1(av2));
	return (1);
}

int		bt_4(char *av1, char *av2, char *av3, char *av4)
{
	if (ft_strcmp(av1, "!") == 0)
	{
		ret = (bt_3(av2, av3, av4));
		if (ret == 1)
			return (0);
		else
			return (1);
	}
	if (ft_strcmp(av1, "(") == 0 && ft_strcmp(av4, ")") == 0)
		return (bt_2(av2, av3));
	return (1);
}

int		bt_5(void)
{
	ft_putendl("Test usage = up to 4 arguments only");
	return (2);
}
