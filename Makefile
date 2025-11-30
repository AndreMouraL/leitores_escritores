NAME = banco_le_esc
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -pthread
SRC_DIR = src/
INCLUDE = -Iinclude

SOURCES = $(SRC_DIR)main.c \
          $(SRC_DIR)init_data.c \
          $(SRC_DIR)consultor.c \
          $(SRC_DIR)transferencia.c \
          $(SRC_DIR)controle.c \
          $(SRC_DIR)monitor.c \
          $(SRC_DIR)tempo.c \
          $(SRC_DIR)utils.c \
          $(SRC_DIR)parse.c

all: $(NAME)

$(NAME):
	@echo " Compilando sistema bancário leitores/escritores..."
	@$(CC) $(CFLAGS) $(INCLUDE) -o $(NAME).exe $(SOURCES)
	@echo " Sistema compilado: $(NAME).exe"

clean:
	@if exist $(NAME).exe del $(NAME).exe
	@echo " Executável removido"

run: all
	@echo " Executando sistema..."
	@.\$(NAME).exe 3 2 410 200 100 200 5

.PHONY: all clean run