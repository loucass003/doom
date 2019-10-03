/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_menu.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 16:44:24 by louali            #+#    #+#             */
/*   Updated: 2019/09/17 14:23:03 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "gui.h"
#include "doom.h"

void		put_menu(t_component *self, t_doom *doom, t_menu *menu)
{
	int		x;
	int		y;

	x = self->bounds.x;
	while (x < self->bounds.x + self->bounds.w)
	{
		y = self->bounds.y;
		while (y < self->bounds.y + self->bounds.h)
		{
			if (menu->name && doom->editor.icone == 1 && ft_strcmp(menu->name, "obj") == 0)
				doom->screen.pixels[(y * doom->screen.width) + x] = 0xFF505050;
			else
				doom->screen.pixels[(y * doom->screen.width) + x] = 0;
			y++;
		}
		x++;
	}
	if (menu->texture)
		apply_surface(&doom->screen, menu->select_file,
			(SDL_Rect) {0, 0, menu->select_file->w, menu->select_file->h},
			(SDL_Rect) {self->bounds.x + 2, self->bounds.y +
			2, 50 - 4, MT_H - 4});
	else 
	{
		for (int x = self->bounds.x; x < self->bounds.x + self->bounds.w; x++)
			for (int y = self->bounds.y; y < self->bounds.y + self->bounds.h; y++)
			{
				doom->screen.pixels[(y * doom->screen.width) + x] = 0xFF505050;
			}
	}
}

void		menu_deroulant(t_component *self, int i, t_texture *f, t_doom *doom)
{
	t_menu		*menu;
	SDL_Surface	*text;
	SDL_Color	color;
	int			x;
	int			y;

	SDL_GetMouseState(&x, &y);
	menu = (t_menu *)self;
	if (x >= self->bounds.x && x <= self->bounds.x + MT_W
	&& y >= (self->bounds.y + MT_H + i)
	&& y <= (self->bounds.y + i + 30 + MT_H))
	{
		color = (SDL_Color) {50, 50, 50, 255};
		menu->select_file = f->text;
		menu->select_pos = i;
	}
	else
		color = (SDL_Color) {250, 250, 250, 255};
	text = TTF_RenderText_Blended(doom->fonts.helvetica, f->name, color);
	apply_surface_blended(&doom->screen, text,
	(SDL_Rect) {0, 0, text->w, text->h},
	(SDL_Rect) {self->bounds.x, self->bounds.y + i + MT_H, MT_W, 30});
}
