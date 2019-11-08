# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2019/11/08 17:15:04 by llelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=doom-nukem
CFLAGS=-Wall -Wextra -flto -O3 -ffast-math
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

INCLUDE += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf openal SDL2_mixer)
LIBS += $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf openal SDL2_mixer) 

PRECOMPILE = mkdir -p $(dir $@)
POSTCOMPILE = sleep 0

ifndef NODEPS

PRECOMPILE += ;mkdir -p $(dir $(DEPSDIR)$*)
CFLAGS += -MT $@ -MMD -MP -MF $(DEPSDIR)$*.Td
POSTCOMPILE += ;mv -f $(DEPSDIR)$*.Td $(DEPSDIR)$*.d && touch $@

endif

all: make_ft | $(NAME)

dev: CFLAGS +=-g
dev: FT_TASK = dev 
dev: re

$(OBJS): Makefile src.mk

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(PRECOMPILE)
	$(CC) $(CFLAGS) $(INCLUDE) $(FT_INC) -I $(INCDIR)  -o $@ -c $<
	@$(POSTCOMPILE)

make_ft: 
	make -j4 -C $(FT)

$(FT_LIB): make_ft


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
	@find srcs -type f -name '*.c' | sed 's/srcs\///g' | sed 's/^/SRC+=/' > src.mk

include $(wildcard $(DEPSDIR)/**/*.d)

.PHONY: clean fclean re all get_files
