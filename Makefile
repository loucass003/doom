# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2019/05/08 16:40:20 by llelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=doom
CFLAGS=-Wall -Wextra -g
LIBS=-lm -lft
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
DEPSDIR = ./deps/

UNAME :=$(shell uname)

INCLUDE += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
LIBS += $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf)

PRECOMPILE = mkdir -p $(dir $@)
POSTCOMPILE = sleep 0

ifndef NODEPS

PRECOMPILE += ;mkdir -p $(dir $(DEPSDIR)$*)
CFLAGS += -MT $@ -MMD -MP -MF $(DEPSDIR)$*.Td
POSTCOMPILE += ;mv -f $(DEPSDIR)$*.Td $(DEPSDIR)$*.d && touch $@

endif

all: $(FT_LIB) $(NAME)

dev: CFLAGS +=-g
dev: FT_TASK = dev 
dev: re

$(OBJS): Makefile src.mk

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(PRECOMPILE)
	$(CC) $(CFLAGS) $(INCLUDE) $(FT_INC) -I $(INCDIR) -o $@ -c $<
	@$(POSTCOMPILE)

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

include $(wildcard $(DEPSDIR)/**/*.d)

.PHONY: clean fclean re all get_files
