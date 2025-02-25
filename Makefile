NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
RM = rm -rf
LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a
HEADER_DIR = inc
HEADER = $(HEADER_DIR)/minishell.h
INCLUDES = -I$(HEADER_DIR) -I$(LIBFT_DIR)
SRC_DIR = src
SRC_FILES = main.c \
			parse/extract_tokens.c \
			parse/extract_commands.c \
			parse/command_cleanup.c \
			parse/command_list.c \
			parse/token_validation.c \
			parse/token_checks.c \
			expand/env_vars_list.c \
			expand/envp_manager.c \
			expand/extract_vars.c \
			expand/expand_token.c \
			expand/concat_vars.c \
			expand/expand_commands.c \
			builtin/echo.c \
			builtin/pwd.c \
			builtin/cd.c \
			builtin/env.c \
			builtin/export.c \
			builtin/unset.c \
			builtin/exit.c \
			execute/launch_builtin.c
SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS_DIR = objs
OBJS = $(addprefix $(OBJS_DIR)/, $(subst /,-, $(SRC_FILES:.c=.o)))

all: $(NAME)

debug: CFLAGS += -g
debug: all

clean:
	$(RM) $(OBJS) $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME) $(LIBFT_LIB)

re: fclean all

$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(LIBFT_LIB):
	@make $(if $(filter debug,$(MAKECMDGOALS)),debug) -C $(LIBFT_DIR)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)/parse-%.o: $(SRC_DIR)/parse/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)/expand-%.o: $(SRC_DIR)/expand/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)/builtin-%.o: $(SRC_DIR)/builtin/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR)/execute-%.o: $(SRC_DIR)/execute/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: all clean fclean re
