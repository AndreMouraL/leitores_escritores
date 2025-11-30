# **************************************************************************** #
#                                                                              #
#    Makefile para Windows - Leitores e Escritores                            #
#                                                                              #
# **************************************************************************** #

NAME = biblioteca
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -pthread
SRC_DIR = src/
INCLUDE = -Iinclude

# Comandos Windows
RM = del /Q
MKDIR = mkdir
RMDIR = rmdir /S /Q

# Lista de arquivos fonte
# Lista de arquivos fonte - ADICIONAR biblioteca.c
SOURCES = $(SRC_DIR)main.c \
          $(SRC_DIR)init_data.c \
          $(SRC_DIR)leitor.c \
          $(SRC_DIR)escritor.c \
          $(SRC_DIR)biblioteca.c \
          $(SRC_DIR)monitor.c \
          $(SRC_DIR)tempo.c \
          $(SRC_DIR)utils.c \
          $(SRC_DIR)parse.c

all: $(NAME)

$(NAME):
	@echo " Compilando sistema de leitores e escritores..."
	@$(CC) $(CFLAGS) $(INCLUDE) -o $(NAME).exe $(SOURCES)
	@echo " Biblioteca compilada: $(NAME).exe"

clean:
	@if exist $(NAME).exe $(RM) $(NAME).exe
	@if exist objects $(RMDIR) objects
	@echo " Arquivos limpos"

fclean: clean

re: fclean all

run: all
	@echo " Executando sistema..."
	@.\$(NAME).exe 3 2 410 200 100 200 5

help:
	@echo "Targets disponíveis:"
	@echo "  all     - Compila o programa"
	@echo "  run     - Compila e executa com parâmetros padrão"
	@echo "  clean   - Remove executável"
	@echo "  re      - Recompila completamente"

.PHONY: all clean fclean re run help