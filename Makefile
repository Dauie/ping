
NAME = ft_ping

CC = gcc

SRCDIR = src

CFLAGS  = -Wall -Werror -Wextra -g

INCL = -I incl

LIBFT_INCL = -I./libft/incl

LIBFT = ./libft/libftprintf.a

MAIN = main.c

SRC = struct_init.c socket.c ping.c checksum.c

SRCFILES = $(addprefix $(SRCDIR)/, $(SRC))

RM = rm -fr

$(NAME):
		$(MAKE) -C ./libft/ re
		$(CC) $(CFLAGS) $(INCL) $(MAIN) $(SRCFILES) $(LIBFT) -o $(NAME)

clean:
		$(RM) $(OBJDIR)

fclean: clean
		$(RM) $(NAME)
		$(RM) libft_malloc.so

re: fclean $(NAME)
