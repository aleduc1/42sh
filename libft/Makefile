# ---------- #
# Debug mode #
# ---------- #

DEBUG = no

# ----- #
# Shell #
# ----- #

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	SHELL = /bin/zsh
else
	SHELL = /bin/bash
endif

# --------- #
# Directory #
# --------- #

SRCDIR = srcs/
OBJDIR = objs/
INCDIR = includes/
LIBDIR = libs/

VPATH = .:\
		objs:\
		srcs:\
# ------------------ #
# Compiler and flags #
# ------------------ #

CC = gcc
ifeq ($(DEBUG), yes)
	CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
else
	CFLAGS = -Wall -Wextra
endif
CPPFLAGS = -I $(INCDIR)

LDFLAGS =

# --------------- #
# Differents name #
# --------------- #

NAME = $(LIBDIR)libft.a

SRCS_NAMES = ft_atoi.c ft_bzero.c ft_fill_str.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
			 ft_isspace.c ft_itoa.c ft_lstadd.c ft_lstdel.c ft_lstdelone.c ft_lst_element_at.c ft_lstextend.c \
			 ft_lstflip.c ft_lstiter.c ft_lstmap.c ft_lstnew.c ft_lstsize.c ft_lsttail.c ft_memalloc.c ft_memccpy.c \
			 ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c ft_memmove.c ft_memset.c ft_mergesort.c \
			 ft_print_word_table.c ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c ft_putnbr.c \
			 ft_putnbr_fd.c ft_putnbrln.c ft_putstr.c ft_putstr_fd.c ft_sort_integer_table.c ft_split_whitespaces.c \
			 ft_strbeginswith.c ft_strcat.c ft_strchr.c ft_strclr.c ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c \
			 ft_strendswith.c ft_strequ.c ft_striter.c ft_striteri.c ft_strjoin.c ft_strjoinfree.c ft_strlcat.c \
			 ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c ft_strnchr.c ft_strncmp.c ft_strncpy.c ft_strnequ.c \
			 ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strrev.c ft_strsplit.c ft_strstr.c ft_strsub.c ft_strsubfree.c \
			 ft_strtrim.c ft_swap.c ft_tolower.c ft_toupper.c get_next_line.c get_end_line.c split_once.c ft_dlstnew.c ft_dlstinserts.c \
			 ft_dlstdel.c ft_dlstprint.c ft_madeby.c ft_debug.c ft_arraydel.c ft_arraydelline.c ft_arraydisplay.c ft_arraydup.c ft_arrayjoin.c \
			 ft_arraylen.c ft_arraysub.c ft_chr_index.c ft_strchr_exist.c ft_numlen.c ft_pow_array_fix.c ft_reverse_tablo.c ft_strsub_tab_fix.c \
			 ft_deplace.c ft_isstrnum.c ft_jointhree.c ft_remove_last_chr.c ft_lpow.c ft_sqrt.c ft_prime.c
OBJS_NAMES = $(SRCS_NAMES:.c=.o)
HEADERS_NAMES = libft.h ft_printf.h get_next_line.h ft_printf.h

SRC = $(addprefix $(SRCDIR), $(SRCS_NAMES))
OBJ = $(addprefix $(OBJDIR), $(OBJS_NAMES))
HEADERS = $(addprefix $(INCDIR), $(HEADERS_NAMES))

# ----------------- #
# Command variables #
# ----------------- #

AR = ar
ARFLAGS = rc
SORT = ranlib
CREATE = mkdir -p
DEL = /bin/rm -rf
PRINT = printf
PHONY = all clean fclean re help
REMOVE = "\r\033[K"
FUNC = "%-60b\r"

# PROGRESS BAR | Original author Cpirlot
T = $(words $(OBJ))
N = 0
C = $(words $N)$(eval N := x $N)
ECHO = "[`expr $C  '*' 100 / $T`%]"

# ----- #
# Rules #
# ----- #

all : $(NAME)

ifeq ($(DEBUG), yes)
	@$(PRINT) ""
else
	@$(PRINT) "\n"
endif

$(NAME) : $(OBJS_NAMES)
	@make -C srcs/ft_printf
	@$(CREATE) $(LIBDIR)
	@$(AR) $(ARFLAGS) $(NAME) objs/*.o #$(OBJ)
	@$(SORT) $(NAME)
	@$(PRINT) $(REMOVE)"Libft.a built\n"

%.o : %.c $(HEADERS)
	@$(CREATE) $(OBJDIR)
#	@$(PRINT) "%-60b\r" "\033[32m $(ECHO)\033[0m $@"
	@$(CC) -o $(OBJDIR)$@ -c $< $(CFLAGS) $(CPPFLAGS)

clean :
	@$(DEL) $(OBJDIR)
	@$(PRINT) ".o file for lib deleted\n"

fclean : clean
	@$(DEL) $(LIBDIR)
	@$(PRINT) "Libft.a destroyed\n"

re : fclean all

help :
	@$(PRINT) "Rules available : all, clean, fclean, re and help\n"

.PHONY : $(PHONY)
