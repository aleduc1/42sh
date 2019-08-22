#include "builtins.h"

void	create_unary_list(t_unary_tab **tab)
{
	if (!((*tab) = (t_unary_tab *)ft_memalloc(sizeof(t_unary_tab) * 16)))
		return ;
	(*tab)[0].str = "-b";
	(*tab)[0].f = &b_test;
	(*tab)[1].str = "-c";
	(*tab)[1].f = &c_test;
	(*tab)[2].str = "-d";
	(*tab)[2].f = &d_test;
	(*tab)[3].str = "-e";
	(*tab)[3].f = &e_test;
	(*tab)[4].str = "-f";
	(*tab)[4].f = &f_test;
	(*tab)[5].str = "-g";
	(*tab)[5].f = &g_test;
	(*tab)[6].str = "-L";
	(*tab)[6].f = &L_test;
	(*tab)[7].str = "-p";
	(*tab)[7].f = &p_test;
	(*tab)[8].str = "-r";
	(*tab)[8].f = &r_test;
	(*tab)[9].str = "-S";
	(*tab)[9].f = &S_test;
	(*tab)[10].str = "-s";
	(*tab)[10].f = &s_test;
	(*tab)[11].str = "-u";
	(*tab)[11].f = &u_test;
	(*tab)[12].str = "-w";
	(*tab)[12].f = &w_test;
	(*tab)[13].str = "-x";
	(*tab)[13].f = &x_test;
	(*tab)[14].str = "-z";
	(*tab)[14].f = &z_test;
	(*tab)[15].str = NULL;
	(*tab)[15].f = NULL;
}

void	create_binary_list(t_unary_tab **tab)
{
	if (!((*tab) = (t_unary_tab *)ft_memalloc(sizeof(t_unary_tab) * 8)))
		return ;
	(*tab)[0].str = "=";
	(*tab)[0].f = &equal_test;
	(*tab)[1].str = "!=";
	(*tab)[1].f = &notequal_test;
	(*tab)[2].str = "-eq";
	(*tab)[2].f = &eq_test;
	(*tab)[3].str = "-ne";
	(*tab)[3].f = &ne_test;
	(*tab)[4].str = "-ge";
	(*tab)[4].f = &ge_test;
	(*tab)[5].str = "-lt";
	(*tab)[5].f = &lt_test;
	(*tab)[6].str = "-le";
	(*tab)[6].f = &le_test;
	(*tab)[7].str = NULL;
	(*tab)[7].f = NULL;
}
