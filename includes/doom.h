/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/23 16:23:29 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <math.h>
# include <stdlib.h>

# include "libft.h"
# include "constants.h"
# include "image.h"
# include "maths/line.h"
# include "bsp.h"

typedef struct		s_stats
{
	int				fps;
	double			avg_ms;
	double			delta;
}					t_stats;

typedef struct		s_fonts
{
	TTF_Font	*helvetica;
}					t_fonts;

typedef struct		s_textures
{
	SDL_Surface		*bricks;
	
}					t_textures;

typedef struct		s_player
{
	t_vec2			pos;
	float			rotation;
}					t_player;

typedef struct		s_doom
{
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	t_img			screen;
	t_bool			running;
	t_stats			stats;
	t_fonts			fonts;
	t_node			*bsp;
	t_player		player;
	t_node			*first_node;
	uint8_t			*rendered_area;
	t_textures		textures;
}					t_doom;

t_bool				init_sdl(t_doom *doom);
t_bool				sdl_quit(t_doom *doom);
void				hook_events(t_doom *doom);
void				render(t_doom *doom);

Uint32				get_surface_pixel(SDL_Surface *surface, int x, int y);
void				apply_surface_blended(t_img *img, SDL_Surface *s,
						SDL_Rect src, SDL_Rect dst);
void				apply_surface(t_img *img, SDL_Surface *s,
						SDL_Rect src, SDL_Rect dst);

#endif
