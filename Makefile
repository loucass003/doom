# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2019/04/11 19:31:16 by llelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=doom
CFLAGS=-Wall -Wextra -g
LIBS=-lm
CC=clang

-include src.mk

OBJS=$(addprefix $(OBJDIR),$(SRC:.c=.o))

# ft library
FT		=./libft/
FT_LIB	=$(addprefix $(FT), libft.a)
FT_INC	=-I ./libft/includes
FT_LNK	=-L ./libft
FT_TASK =

SRCDIR	=./srcs/
INCDIR	=./includes/
OBJDIR	=./objs/

UNAME :=$(shell uname)

INCLUDE += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LIBS += $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

all: $(FT_LIB) $(NAME)


dev: CFLAGS +=-g
dev: FT_TASK = dev 
dev: re

$(OBJS): Makefile src.mk

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) $(FT_INC) -I $(INCDIR) -o $@ -c $<

$(FT_LIB):
	make -j4 -C $(FT)

$(NAME): $(OBJS) $(FT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(FT_LNK) $(INCLUDE) $(LIBS) $(OBJS) $(FT_LIB)

clean:
	make -C $(FT) clean
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)
	make -C $(FT) fclean

re: fclean $(NAME)


get_files:
	@find srcs -type f | sed 's/srcs\///g' | sed 's/^/SRC+=/' > src.mk

.PHONY: clean fclean re all get_files
