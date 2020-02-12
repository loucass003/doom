/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 22:01:13 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/12 16:00:52 by lloncham         ###   ########.fr       */
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
	//printf("%f\n", doom->stats.delta );
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

void			init_gameover(t_doom *doom)
{
	t_gameover		*gameover;

	gameover = &doom->gameover;
	gameover->kill = 0;
	gameover->bullets = 0;
	gameover->weapon = 0;
	gameover->totaldamage = 0;
}

void			pre_init(t_doom *doom)
{
	init_player(doom);
	init_gameover(doom);
}


void			game_loop(t_doom *doom)
{
	register_guis(doom);
	
	if (check_ressources_errors(doom))
		set_gui(doom, GUI_RESSOURCES);
	else
	{
		pre_init(doom);
		set_gui(doom, GUI_MAIN_MENU);
		
	}
//	default_renderables(doom);

	
	
	
	while (doom->running)
	{
		doom->gdata = (t_gdata){
			.todo_triangles = 0,
			.all_called = FALSE
		};
		SDL_RenderClear(doom->renderer);
		clear_image(&doom->screen);
	//	clear_image(&doom->screen_transparency);
		//fill_color(&doom->screen, 0xFF);
		hook_events(doom);
		render(doom);
		if (doom->help == TRUE)
		{
			t_img			*help;
			help = doom->res_manager.ressources->values[21]->data.texture;
			apply_image_to_image(&doom->screen, help,
				(SDL_Rect) {0, 0, help->width, help->height},
				(SDL_Rect) {20, 20, S_WIDTH - 40, S_HEIGHT - 40});
		}
		//DO RENDERING HERE !
	
		render_debug(doom);
		update_image(&doom->screen);
		apply_image(&doom->screen, NULL);
	//	update_image(&doom->screen_transparency);
	//	apply_image(&doom->screen_transparency, NULL);
		SDL_RenderPresent(doom->renderer);
		update_fps(doom);
		update_sounds(doom);
	/* 	 if (++i > 100)
			exit(0); */
	}
	// FREE TEXTURES FROM LOAD_ALL
}
