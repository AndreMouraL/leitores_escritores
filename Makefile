NAME = biblioteca
CC = gcc
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -g -pthread

SRC_DIR = src/
OBJ_DIR = objects/

SRC =	main \
		init_data \
		leitor \
		escritor \
		biblioteca \
		monitor \
		tempo \
		utils \
		parse

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC)))
OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c 
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@ -Iinclude

clean:
	@$(RM) $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME) 

re: fclean all

run: all
	@./$(NAME) 3 2 410 200 100 200 5

.PHONY: all clean fclean re run