/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 20:10:35 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 12:14:58 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <SDL_image.h>
#include "image.h"
#include "doom.h"

static t_bool	load_fonts(t_doom *doom)
{
	if (!(doom->fonts.helvetica = TTF_OpenFont(FONT_VERAMONO, 25)))
		return (FALSE);
	return (TRUE);
}

t_bool			init_sdl(t_doom *doom)
{
	Uint32	n;

	n = 1;
	if (SDL_Init(SDL_INIT_VIDEO) < 0
		|| IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG
		|| TTF_Init() < 0)
		return (sdl_quit(doom));
	if (!(doom->win = SDL_CreateWindow("|| DOOM ||", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN)))
		return (sdl_quit(doom));
	if (!(doom->renderer = SDL_CreateRenderer(doom->win, -1,
		SDL_RENDERER_ACCELERATED)))
		return (sdl_quit(doom));
	SDL_RenderSetLogicalSize(doom->renderer, S_WIDTH, S_HEIGHT);
	SDL_SetRenderDrawBlendMode(doom->renderer, SDL_BLENDMODE_NONE);
	if (!create_image(doom->renderer, S_WIDTH, S_HEIGHT, &doom->screen))
		return (sdl_quit(doom));
	doom->main_context.image = &doom->screen;
	if (!load_fonts(doom))
		return (sdl_quit(doom));
	if ((doom->trigger_event = SDL_RegisterEvents(1)) == -n)
		return (FALSE);
	doom->fullscreen = FALSE;
	return (TRUE);
}

t_bool			sdl_quit(t_doom *doom)
{
	destroy_image(&doom->screen);
	TTF_CloseFont(doom->fonts.helvetica);
	if (doom->renderer)
		SDL_DestroyRenderer(doom->renderer);
	if (doom->win)
		SDL_DestroyWindow(doom->win);
	TTF_Quit();
	SDL_Quit();
	return (FALSE);
}
