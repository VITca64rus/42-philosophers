NAME = philo
CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRC  =  	main.c		ft_atoi.c		ft_create.c		ft_live.c		ft_utils.c		ft_moni.c

OBJ = $(patsubst %.c,%.o,$(SRC))
INCLUDES = ft_philosophers.h

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.c $(INCLUDES)
	gcc $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)
	
re : fclean all

.PHONY : all clean fclean re