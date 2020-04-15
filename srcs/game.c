/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:01:13 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 17:51:39 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "camera.h"
#include "player.h"

static void		render_debug(t_doom *doom)
{
	const SDL_Color	color = {255, 255, 255, 0};
	SDL_Surface		*text;

	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str(doom->stats.fps).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect)
		{0, 0, text->w, text->h},
		(SDL_Rect){0, S_HEIGHT - 80, 40, 40});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(doom->fonts.helvetica,
		ft_int_to_str((int)doom->stats.avg_ms).str, color);
	apply_surface_blended(&doom->screen, text, (SDL_Rect)
		{0, 0, text->w, text->h},
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

void			update_sounds(t_doom *doom)
{
	int		i;
	ALint	status;

	i = 0;
	while (i < MAX_SOUNDS)
	{
		if (doom->audio.source_status[i] == TRUE)
		{
			alGetSourcei(doom->audio.source[i], AL_SOURCE_STATE, &status);
			if (status != AL_PLAYING)
			{
				doom->audio.source_status[i] = FALSE;
			}
		}
		++i;
	}
}

void			loop_running(t_doom *doom)
{
	t_img			*help;

	SDL_RenderClear(doom->renderer);
	clear_image(&doom->screen);
	hook_events(doom);
	render(doom);
	if (doom->help == TRUE)
	{
		help = doom->res_manager.ressources->values[21]->data.texture;
		apply_image_to_image(&doom->screen, help,
			(SDL_Rect) {0, 0, help->width, help->height},
			(SDL_Rect) {20, 20, S_WIDTH - 40, S_HEIGHT - 40});
	}
	render_debug(doom);
	update_image(&doom->screen);
	apply_image(&doom->screen, NULL);
	SDL_RenderPresent(doom->renderer);
	update_fps(doom);
	update_sounds(doom);
}

void			game_loop(t_doom *doom)
{
	register_guis(doom);
	if (check_ressources_errors(doom))
		set_gui(doom, GUI_RESSOURCES);
	else
		set_gui(doom, GUI_MAIN_MENU);
	init_gameover(doom);
	while (doom->running)
		loop_running(doom);
}
