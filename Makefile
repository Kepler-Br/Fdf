# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nsena <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/21 16:20:22 by nsena             #+#    #+#              #
#    Updated: 2019/10/21 18:01:51 by nsena            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COLOR_NONE = \e[0m
COLOR_WHITE = \e[1;37m
COLOR_BLUE = \e[1;34m
COLOR_GREEN = \e[1;32m
COLOR_PURPLE = \e[1;35m

NAME = fdf
CCFL = gcc -Wall -Wextra -Werror
SRCS = main.c main_loop1.c linear_alg.c renderer.c image.c landscape.c math.c read_map.c gnl.c ft_make_t_vector2.c ft_make_t_vector.c ft_destroy_t_vector.c read_map_2.c process_input.c
OBJ = $(patsubst %.c,%.o,$(SRCS))
INCL = -I. -Ilibft -Iminilibx_macos
FRAMEWORKS = -framework OpenGL -framework AppKit
LIBRARIES = -Llibft -Lminilibx_macos -lft -lmlx -lm
MINILIBX_DIR = ./minilibx_macos
LIBFT_DIR = ./libft

all: $(NAME)

$(NAME): libftt minilibx $(OBJ) 
	@printf "$(COLOR_GREEN)Linking objects together...$(COLOR_NONE)\n"
	@$(CCFL) $(LIBRARIES) $(INCL) $(FRAMEWORKS) $(OBJ)
	@mv ./a.out $(NAME)

%.o: %.c
	@printf "$(COLOR_GREEN)Compiling $(COLOR_PURPLE)$<$(COLOR_GREEN) to $(COLOR_PURPLE)$@$(COLOR_GREEN)...$(COLOR_NONE)\n"
	@$(CCFL) $(INCL) $< -c -o $@

clean:
	@printf "$(COLOR_GREEN)Cleaning object files...$(COLOR_NONE)\n"
	@/bin/rm -f *.o
	@$(MAKE) -C $(MINILIBX_DIR) clean
	@$(MAKE) -C $(LIBFT_DIR) clean

minilibx:
	@printf "$(COLOR_GREEN)Compiling minilibx...$(COLOR_NONE)\n"
	@$(MAKE) -C $(MINILIBX_DIR)

libftt:
	@printf "$(COLOR_GREEN)Compiling libft...$(COLOR_NONE)\n"
	@$(MAKE) -C $(LIBFT_DIR)

fclean: clean
	@printf "$(COLOR_GREEN)Cleaning executable...$(COLOR_NONE)\n"
	@/bin/rm -f $(NAME)
	@/bin/rm -f $(MINILIBX_DIR)/libmlx.a
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean $(NAME)
