# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2020/04/19 20:51:11 by llelievr         ###   ########.fr        #
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

INCLUDE += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf openal)
LIBS += $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf openal)

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
	@$(call run_and_test, $(CC) $(CFLAGS) $(INCLUDE) $(FT_INC) -I $(INCDIR)  -o $@ -c $<)
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


COM_COLOR   = \033[36m
OBJ_COLOR   = \033[m
OK_COLOR    = \033[0;36m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

OK_STRING    = "OK"
ERROR_STRING = "ERROR"
WARN_STRING  = "WARNING"
COM_STRING   = "Compiling"

define run_and_test
	printf "%b" "$(COM_COLOR)$(COM_STRING) $(OBJ_COLOR)$(@F)$(NO_COLOR)\r"; \
	$(1) 2> $@.log; \
	RESULT=$$?; \
	if [ $$RESULT -ne 0 ]; then \
		printf "%-78b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $<" "[ $(ERROR_COLOR)$(ERROR_STRING)$(NO_COLOR) ]\n"; \
	elif [ -s $@.log ]; then \
		printf "%-78b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $<" "[ $(WARN_COLOR)$(WARN_STRING)$(NO_COLOR) ]\n"; \
	else  \
		printf "%-78b%b" "$(COM_COLOR)$(COM_STRING)$(OBJ_COLOR) $(<)" "[ $(OK_COLOR)$(OK_STRING)$(NO_COLOR) ]\n"; \
	fi; \
	cat $@.log; \
	rm -f $@.log; \
	exit $$RESULT
endef
