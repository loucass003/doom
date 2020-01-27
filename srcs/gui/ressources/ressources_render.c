/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ressources_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 12:28:17 by lloncham          #+#    #+#             */
/*   Updated: 2020/01/24 13:56:52 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gui.h"
#include "doom.h"

void			render_page_label(t_gui *self, t_doom *doom)
{
	const SDL_Color	color = (SDL_Color){255, 255, 255, 0};
	SDL_Surface		*t;

	(void)self;
	t = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(doom->res_manager.page).str, color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - 60 - t->w,
		S_HEIGHT - 60 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica, "/", color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, S_HEIGHT - 60 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(get_pages_count(&doom->res_manager)).str, color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 + 40 - t->w,
		S_HEIGHT - 60 + t->h / 2, t->w, t->h});
	SDL_FreeSurface(t);
	t = TTF_RenderText_Blended(doom->fonts.helvetica,
		"RESSOURCES MANAGER", color);
	apply_surface_blended(&doom->screen, t, (SDL_Rect){0, 0, t->w, t->h},
		(SDL_Rect){S_WIDTH_2 - t->w / 2, 15, t->w, t->h});
	SDL_FreeSurface(t);
}

void			render_page(t_gui *self, t_doom *doom)
{
	const t_vec2			pos = get_mouse_pos(doom);
	t_ressource_manager		res;
	t_ressource				*ressou;
	int						i;
	int						color;

	(void)self;
	i = -1;
	res = doom->res_manager;
	while (++i < PAGE_SIZE)
	{
		if (i < (res.ressources->len
			- (res.page * PAGE_SIZE)))
		{
			ressou = res.ressources->values[res.page * PAGE_SIZE + i];
			color = ressou->loaded ? 0xFF00FF00 : 0xFFFF0000;
			if (in_bounds((SDL_Rect)
				{ S_WIDTH_2 - 356, 53 + i * 30, 584, 28 }, pos))
				color = 0xFFFFFFFF;
			draw_rect(&doom->screen, (SDL_Rect)
				{ S_WIDTH_2 - 356, 53 + i * 30, 584, 28 }, color);
		}
	}
}

void			g_ressources_render(t_gui *self, t_doom *doom)
{
	fill_color(&doom->screen, 0xFF252525);
	render_page(self, doom);
	render_components(doom, self);
	render_page_label(self, doom);
	apply_select_value(self, doom);
}
