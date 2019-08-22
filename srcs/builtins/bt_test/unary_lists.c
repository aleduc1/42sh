#include "builtins.h"

void	create_unary_list(t_unary_tab **tab_lst)
{
	if (!((*tab_lst) = (t_unary_tab *)ft_memalloc(sizeof(t_unary_tab) * 16)))
		return ;
	(*tab_lst)[0].str = "-b";
	(*tab_lst)[0].f = &b_test;
	(*tab_lst)[1].str = "-c";
	(*tab_lst)[1].f = &c_test;
	(*tab_lst)[2].str = "-d";
	(*tab_lst)[2].f = &d_test;
	(*tab_lst)[3].str = "-e";
	(*tab_lst)[3].f = &e_test;
	(*tab_lst)[4].str = "-f";
	(*tab_lst)[4].f = &f_test;
	(*tab_lst)[5].str = "-g";
	(*tab_lst)[5].f = &g_test;
	(*tab_lst)[6].str = "-L";
	(*tab_lst)[6].f = &L_test;
	(*tab_lst)[7].str = "-p";
	(*tab_lst)[7].f = &p_test;
	(*tab_lst)[8].str = "-r";
	(*tab_lst)[8].f = &r_test;
	(*tab_lst)[9].str = "-S";
	(*tab_lst)[9].f = &S_test;
	(*tab_lst)[10].str = "-s";
	(*tab_lst)[10].f = &s_test;
	(*tab_lst)[11].str = "-u";
	(*tab_lst)[11].f = &u_test;
	(*tab_lst)[12].str = "-w";
	(*tab_lst)[12].f = &w_test;
	(*tab_lst)[13].str = "-x";
	(*tab_lst)[13].f = &x_test;
	(*tab_lst)[14].str = "-z";
	(*tab_lst)[14].f = &z_test;
	(*tab_lst)[15].str = NULL;
	(*tab_lst)[15].f = NULL;
}

void	create_binary_list(t_unary_tab **tab_lst)
{
	if (!((*tab_lst) = (t_unary_tab *)ft_memalloc(sizeof(t_unary_tab) * 8)))
		return ;
	(*tab_lst)[0].str = "=";
	(*tab_lst)[0].f = &equal_test;
	(*tab_lst)[1].str = "!=";
	(*tab_lst)[1].f = &notequal_test;
	(*tab_lst)[2].str = "-eq";
	(*tab_lst)[2].f = &eq_test;
	(*tab_lst)[3].str = "-ne";
	(*tab_lst)[3].f = &ne_test;
	(*tab_lst)[4].str = "-ge";
	(*tab_lst)[4].f = &ge_test;
	(*tab_lst)[5].str = "-lt";
	(*tab_lst)[5].f = &lt_test;
	(*tab_lst)[6].str = "-le";
	(*tab_lst)[6].f = &le_test;
	(*tab_lst)[7].str = NULL;
	(*tab_lst)[7].f = NULL;
}
