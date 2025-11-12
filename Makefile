# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME		= 	ft_traceroute

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

SRC			= 	.

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC			=	main.c			\
				parse.c			\
				socket.c		\
				traceroute.c	\
				utils.c

SRCDIR		= srcs
SRCS		= $(addprefix $(SRCDIR)/, $(SRC))

OBJDIR		= .obj
OBJS		= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

# -=-=-=-=-    INCLUDES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

HEADERS		=	./incs/ft_traceroute.h ./libft/libft.h

LIBFT		=	./libft/libft.a

MAKE		=	Makefile

# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pedantic -g# -fsanitize=address
INCLUDES	= -I

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: make_libft $(NAME)

$(NAME): $(OBJS) $(HEADERS) $(SRCS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

make_libft:
	@make -C libft

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) Makefile
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES)./libft/ -c $< -o $@
	
clean:
	@/bin/rm -fr $(OBJDIR)
	@make -C ./libft clean

fclean: clean
	@/bin/rm -f $(NAME)
	@make -C ./libft fclean

re: fclean all

.PHONY:  all clean fclean re make_libft
