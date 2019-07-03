/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:01:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/26 11:52:52 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void			render_debug(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(doom->stats.fps).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){0, 600, 40, 40});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str((int)doom->stats.avg_ms).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){0, 560, 40, 40});
	SDL_FreeSurface(text);
}

static void		update_fps(t_doom *doom)
{
	static double		t1 = 0;
	static double		t2 = 0;
	static double		t3 = 0;

	t2 = t1;
	t1 = SDL_GetTicks();
	doom->stats.delta = (t1 - t2) / 1000.;
	if (t3 + 100 < t1)
	{
		t3 = t1;
		doom->stats.fps = 1 / doom->stats.delta;
		doom->stats.avg_ms = 1000
			/ (doom->stats.fps == 0 ? 0.001 : doom->stats.fps);
	}
}

void			game_loop(t_doom *doom)
{
	doom->player.pos = (t_vec2){ 0, 0 };
	register_guis(doom);
	set_gui(doom, GUI_MAIN_MENU);
	load_all(doom);
	while (doom->running)
	{
		clear_image(&doom->screen);
		//fill_color(&doom->screen, 0xFF0000);
		render(doom);
		//DO RENDERING HERE !
		hook_events(doom);
		render_debug(doom);
		update_image(&doom->screen);
		apply_image(&doom->screen, NULL);
		SDL_RenderPresent(doom->renderer);
		update_fps(doom);
	}
	// FREE TEXTURES FROM LOAD_ALL
}
