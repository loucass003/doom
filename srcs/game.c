/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:01:13 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/31 16:45:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "camera.h"
#include "player.h"

static void			render_debug(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(doom->stats.fps).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){0, S_HEIGHT - 80, 40, 40});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str((int)doom->stats.avg_ms).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){0, S_HEIGHT - 40, 40, 40});
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
	if (t3 + 1000 < t1)
	{
		t3 = t1;
		doom->stats.fps = 1 / doom->stats.delta;
		doom->stats.avg_ms = 1000
			/ (doom->stats.fps == 0 ? 0.001 : doom->stats.fps);
	}
}

void			game_loop(t_doom *doom)
{
	init_openal(doom);
	load_wav();

	register_guis(doom);

	doom->player.camera.projection = projection_matrix();
	doom->player.pos = (t_vec3){ 11, 5, 4 };
	doom->player.rotation.y = 4.879;
	doom->player.rotation.x = 0;
	doom->player.rotation.z = 0;
	doom->main_context.camera = &doom->player.camera;
	update_player_camera(&doom->player);
	set_gui(doom, GUI_MAIN_MENU);
	int  i = 0;
	load_all(doom);
	while (doom->running)
	{
		SDL_RenderClear(doom->renderer);
		clear_image(&doom->screen);
		//fill_color(&doom->screen, 0xFF);
		render(doom);
		//DO RENDERING HERE !
		hook_events(doom);
		render_debug(doom);
		update_image(&doom->screen);
		apply_image(&doom->screen, NULL);
		SDL_RenderPresent(doom->renderer);
		update_fps(doom);
	/* 	 if (++i > 100)
			exit(0); */
	}
	// FREE TEXTURES FROM LOAD_ALL
}
