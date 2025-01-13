NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L$(LIBFT_DIR) -lft
LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

MINISHELL_DIR = mandatory

HEADER_DIR = $(MINISHELL_DIR)/inc
HEADER = $(HEADER_DIR)/minishell.h
INCLUDES = -I$(HEADER_DIR) -I$(LIBFT_DIR)

SRC_DIR = $(MINISHELL_DIR)/src
SRC_FILES = server.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJS_DIR = $(MINISHELL_DIR)/objs
OBJS = $(addprefix $(OBJS_DIR)/, $(SRC_FILES:.c=.o))

all: $(NAME)

clean:
	rm -rf $(OBJS) $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME) $(LIBFT_LIB)

re: fclean all

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(SERVER_NAME)

$(LIBFT_LIB):
	@make -C $(LIBFT_DIR)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re
