/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_menu.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 14:21:03 by louali            #+#    #+#             */
/*   Updated: 2019/05/17 14:21:07 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void	c_render(t_doom *doom, t_component *self)
{
	t_menu		*menu;
	t_files		*file;
	int			i;

	menu = (t_menu *)self;
	put_menu(self, doom, menu);
	file = menu->name_file;
	if (!menu->open)
		return ;
	i = 0;
	while (file)
	{
		menu_deroulant(self, i, file, doom);
		i += 30;
		file = file->next;
	}
}

void		close_menu(t_component *self, t_vec2 pos, t_menu *m, SDL_Surface *t)
{
	if (pos.x >= self->bounds.x && pos.x <= self->bounds.x + MT_W
	&& pos.y >= (self->bounds.y + MT_H + m->select_pos)
	&& pos.y <= (self->bounds.y + m->select_pos + 30 + MT_H))
	{
		t = IMG_Load(m->select_file);
		t = SDL_ConvertSurfaceFormat(t,
		SDL_PIXELFORMAT_ARGB8888, 0);
		m->texture = t;
		m->open = FALSE;
		self->bounds.h = MT_H;
	}
}

static void	c_on_click(t_component *self, t_vec2 pos, t_doom *doom)
{
	t_menu			*menu;
	SDL_Surface		*texture;

	menu = (t_menu *)self;
	if (pos.x >= self->bounds.x && pos.x <= self->bounds.x + MT_W
	&& pos.y >= self->bounds.y && pos.y <= self->bounds.y + MT_H)
	{
		if (menu->click == 1)
		{
			menu->click = 0;
			menu->color = 0xFFFFFF;
			menu->open = FALSE;
			self->bounds.h = MT_H;
		}
		else
		{
			menu->click = 1;
			menu->color = 100100100;
			menu->open = FALSE;
			self->bounds.h = MT_H;
		}
	}
	close_menu(self, pos, menu, texture);
}

static void	c_on_mouse_move(t_component *self, t_vec2 pos, t_doom *doom)
{
	t_menu	*menu;

	menu = (t_menu*)self;
	if (pos.x >= self->bounds.x && pos.y >= self->bounds.y
		&& pos.y < self->bounds.y + self->bounds.h
		&& pos.x < self->bounds.x + self->bounds.w)
	{
		menu->open = TRUE;
		self->bounds.h = MT_H + menu->files_count * 30;
	}
	else
	{
		menu->open = FALSE;
		self->bounds.h = MT_H;
	}
}

t_component	*create_menu(SDL_Rect bounds, char *s, char *s2)
{
	t_menu		*btn;
	t_files		*list;

	list = NULL;
	if (!(btn = (t_menu *)malloc(sizeof(t_menu))))
		return (NULL);
	ft_bzero(btn, sizeof(t_menu));
	btn->super = (t_component) { .bounds = bounds, .type = C_BUTTON,
		.render = c_render, .on_click = c_on_click,
		.on_mouse_move = c_on_mouse_move};
	btn->open = FALSE;
	find_files(&btn->name_file, &btn->files_count, s, s2);
	return ((t_component *)btn);
}
