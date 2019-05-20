/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_menu.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 16:44:24 by louali            #+#    #+#             */
/*   Updated: 2019/05/17 16:46:33 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			doom->screen.pixels[(y * doom->screen.width) + x] = menu->color;
			y++;
		}
		x++;
	}
	if (menu->texture)
		apply_surface(&doom->screen, menu->texture,
			(SDL_Rect) {0, 0, menu->texture->w, menu->texture->h},
			(SDL_Rect) {self->bounds.x + 2, self->bounds.y +
			2, 75 - 4, MT_H - 4});
}

void		menu_deroulant(t_component *self, int i, t_files *f, t_doom *doom)
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
		menu->select_file = f->s;
		menu->select_pos = i;
	}
	else
		color = (SDL_Color) {250, 250, 250, 255};
	text = TTF_RenderText_Blended(doom->fonts.helvetica, f->s, color);
	apply_surface_blended(&doom->screen, text,
	(SDL_Rect) {0, 0, text->w, text->h},
	(SDL_Rect) {self->bounds.x, self->bounds.y + i + MT_H, MT_W, 30});
}
