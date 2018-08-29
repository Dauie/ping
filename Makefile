
NAME = ping

CC = gcc

SRCDIR = src

CFLAGS  = -Wall -Werror -Wextra

INCL = -I incl

LIBFT_INCL = -I./libft/incl

LIBFT = ./libft/libftprintf.a

MAIN = main.c

SRCFILES =  ping.c recv_ping.c send_ping.c signal.c handle_response.c

SRC = $(addprefix $(SRCDIR)/, $(SRCFILES))

RM = rm -fr

$(NAME):
		$(MAKE) -C ./libft/ re
		$(CC) $(CFLAGS) $(INCL) $(MAIN) $(SRC) $(LIBFT) -o $(NAME)

clean:
		$(RM) $(NAME)

fclean: clean
		make fclean -C libft

re: fclean $(NAME)
