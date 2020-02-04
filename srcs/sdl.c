/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 20:10:35 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/04 16:14:04 by louali           ###   ########.fr       */
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

static t_bool	load_textures(t_doom *doom)
{
	if (!(doom->textures.bricks = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/block00007.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);
	if (!(doom->textures.test = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/test.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);
	if (!(doom->textures.sprite = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/enemy_1.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);  
	if (!(doom->textures.machin = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/sprite.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);  
	if (!(doom->textures.gun0 = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/gun0.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);  
	if (!(doom->textures.medkit = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/medkit.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);  
	if (!(doom->textures.ammo1 = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/ammo1.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);  
	if (!(doom->textures.axe = SDL_ConvertSurfaceFormat(IMG_Load("assets/textures/axe.png"), SDL_PIXELFORMAT_ARGB8888, 0)))
		return (FALSE);  
	return (TRUE);
}

t_bool			init_sdl(t_doom *doom)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG
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
	SDL_EventState(SDL_DROPFILE, SDL_DISABLE);
	if (!create_image(doom->renderer, S_WIDTH, S_HEIGHT, &doom->screen))
		return (sdl_quit(doom));
	if (!create_image(doom->renderer, S_WIDTH, S_HEIGHT, &doom->screen_transparency))
		return (sdl_quit(doom));
	doom->main_context.image = &doom->screen;
	if (!load_fonts(doom))
		return (sdl_quit(doom));
	if (!load_textures(doom))
		return (sdl_quit(doom));
	doom->fullscreen = FALSE;
	return (TRUE);
}

t_bool			sdl_quit(t_doom *doom)
{
	if (doom->renderer)
		SDL_DestroyRenderer(doom->renderer);
	if (doom->win)
		SDL_DestroyWindow(doom->win);
	destroy_image(&doom->screen);
	TTF_Quit();
	SDL_Quit();
	return (FALSE);
}
