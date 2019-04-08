# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2019/04/07 23:38:04 by llelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=doom
SDL=${HOME}/.brew/Cellar/sdl2/2.0.9
SDL_IMG=${HOME}/.brew/Cellar/sdl2_image/2.0.4
SDL_TTF=${HOME}/.brew/Cellar/sdl2_ttf/2.0.14
CFLAGS=-Wall -Wextra -ffast-math -O3 -flto
LIBS=-lSDL2_image -lSDL2_ttf -lSDL2
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

ifeq ($(UNAME), Linux)
	INCLUDE += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer SDL2_ttf)
else
	INCLUDE += -Iinclude -I$(SDL_IMG)/include/SDL2 -I$(SDL_TTF)/include/SDL2 -I$(SDL)/include/SDL2
	LIBS+=-L$(SDL)/lib -lSDL2 -L$(SDL_IMG)/lib -lSDL2_image -L$(SDL_TTF)/lib -lSDL2_ttf
endif

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
	$(CC) $(CFLAGS) -o $(NAME) $(FT_LNK) $(INCLUDE) $(LIBS) -lm $(OBJS) $(FT_LIB)

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
