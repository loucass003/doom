/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_menu.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 14:21:03 by louali            #+#    #+#             */
/*   Updated: 2019/11/08 15:34:51 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

static void	c_render(t_doom *doom, t_component *self)
{
	t_menu		*menu;
	t_texture	*file;
	int			i;

	menu = (t_menu *)self;
	put_menu(self, doom, menu);
	if (menu->name && doom->editor.icone == 1 && ft_strcmp(menu->name, "obj") == 0)
		doom->editor.objet = menu->texture;
	if (menu->name && ft_strcmp(menu->name, "wall") == 0)
		doom->editor.texture = menu->texture;
	if (ft_strcmp(menu->name, "wall") == 0)
		file = doom->wall;
	else if (ft_strcmp(menu->name, "obj") == 0)
		file = doom->obj;
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

void		close_menu(t_component *self, t_vec2 pos, t_menu *m, SDL_Surface *t, t_doom *doom)
{
	if (pos.x >= self->bounds.x && pos.x <= self->bounds.x + MT_W
	&& pos.y >= (self->bounds.y + MT_H + m->select_pos)
	&& pos.y <= (self->bounds.y + m->select_pos + 30 + MT_H))
	{
		doom->open = 0;
		m->texture = m->select_file;
		m->open = FALSE;

		self->bounds.h = MT_H;

	}
}

static t_bool	on_menu_event(t_component *self, SDL_Event *event, t_doom *doom)
{
	t_menu	*menu;
	SDL_Surface		*texture;
	t_vec2			pos;

	menu = (t_menu*)self;
	pos = (t_vec2){ event->motion.x, event->motion.y };
	if (event->type == SDL_MOUSEMOTION)
	{
		if (pos.x >= self->bounds.x && pos.y >= self->bounds.y
		&& pos.y < self->bounds.y + self->bounds.h
		&& pos.x < self->bounds.x + self->bounds.w)
		{
			menu->open = TRUE;
			doom->open = 1;
			if (ft_strcmp(menu->name, "wall") == 0)
				self->bounds.h = MT_H + doom->wall->count * 30;
			else if (ft_strcmp(menu->name, "obj") == 0)
				self->bounds.h = MT_H + doom->obj->count * 30;			
		}
		else
		{
			doom->open = 0;
			menu->open = FALSE;
			self->bounds.h = MT_H;
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP)
	{
		texture = NULL;
		if (in_bounds(self->bounds, pos) && self->perform_action != NULL)
			self->perform_action(self, doom);
		close_menu(self, pos, menu, texture, doom);
	}
	return (FALSE);
}

t_component	*create_menu(SDL_Rect bounds, char *s, t_doom *doom)
{
	t_menu		*btn;
	t_files		*list;

	list = NULL;
	if (!(btn = (t_menu *)malloc(sizeof(t_menu))))
		return (NULL);
	ft_bzero(btn, sizeof(t_menu));
	btn->super = (t_component) { .enabled = TRUE, .visible = TRUE, .bounds = bounds, .type = C_BUTTON,
		.render = c_render, .on_event = on_menu_event };
	btn->open = FALSE;
	doom->open = 0;
	btn->color = 0xFFFFFFFF;
	btn->name = s;
	return ((t_component *)btn);
}
