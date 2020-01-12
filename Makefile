# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2020/01/10 15:12:02 by llelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=doom-nukem
GFLAGS=-Wall -Wextra
OPTI_FLAGS=-flto -O3 -march=native -ffast-math
CFLAGS=$(GFLAGS) $(OPTI_FLAGS)
LIBS=-lm -lft
CC=clang

-include src.mk

OBJS=$(addprefix $(OBJDIR),$(SRC:.c=.o))

# ft library
FT		=./libft/
FT_LIB	=$(addprefix $(FT), libft.a)
FT_INC	=-I ./libft/includes
FT_LNK	=-L ./libft

SRCDIR	=./srcs/
INCDIR	=./includes/
OBJDIR	=./objs/
DEPSDIR = ./deps/

INCLUDE += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf openal SDL2_mixer)
LIBS += $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf openal SDL2_mixer) 

PRECOMPILE = mkdir -p $(dir $@)
POSTCOMPILE = sleep 0

all: make_ft | $(NAME)

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS = $(GFLAGS) -g
endif

ifndef NODEPS

PRECOMPILE += ;mkdir -p $(dir $(DEPSDIR)$*)
CFLAGS += -MT $@ -MMD -MP -MF $(DEPSDIR)$*.Td
POSTCOMPILE += ;mv -f $(DEPSDIR)$*.Td $(DEPSDIR)$*.d && touch $@

endif

$(OBJS): Makefile

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(PRECOMPILE)
	$(CC) $(CFLAGS) $(INCLUDE) $(FT_INC) -I $(INCDIR)  -o $@ -c $<
	@$(POSTCOMPILE)

make_ft: 
	make -C $(FT)

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


include $(shell find $(DEPSDIR) -iname "*.d")

.PHONY: clean fclean re all get_files
