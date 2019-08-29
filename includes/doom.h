/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 16:03:52 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_mixer.h>

# include "libft.h"
# include "constants.h"
# include "image.h"
# include "obj.h"
# include "collision.h"
# include "polygon.h"
# include "editor.h"
# include "gui.h"
# include "player.h"

typedef struct		s_light
{
	t_vec3			polision;
	t_color			color;
}					t_light;

typedef struct		s_stats
{
	int				fps;
	double			avg_ms;
	double			delta;
}					t_stats;

typedef struct		s_fonts
{
	TTF_Font		*helvetica;
}					t_fonts;

typedef struct		s_textures
{
	SDL_Surface		*bricks;
}					t_textures;

typedef struct		s_doom
{
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	t_img			screen;
	t_bool			running;
	t_stats			stats;
	t_fonts			fonts;
	t_polygons		*polygons;
	t_player		player;
	t_vec2			mouse;
	float			*rendered_area;
	t_textures		textures;
	t_gui			guis[GUI_COUNT];
	int				current_gui;
	t_editor		editor;
	char			*obj_working_dir;
	t_obj			*obj_test;
	t_bool			collision;
	int				menu;
	t_line			linetodel;
	t_texture       *wall;
    t_texture       *obj;
    t_texture       *icons;
	int				open;
}					t_doom;



t_bool				init_sdl(t_doom *doom);
t_bool				sdl_quit(t_doom *doom);
void				hook_events(t_doom *doom);
void				render(t_doom *doom);
void				game_loop(t_doom *doom);

Uint32				get_surface_pixel(SDL_Surface *surface, int x, int y);
void				apply_surface_blended(t_img *img, SDL_Surface *s,
						SDL_Rect src, SDL_Rect dst);
void				apply_surface(t_img *img, SDL_Surface *s,
						SDL_Rect src, SDL_Rect dst);



t_mat4				projection_matrix(t_doom *doom);
void				update_maxtrix(t_doom *doom);
void				draw_line(t_img *img, t_pixel p0, t_pixel p1);
void				draw_circle(t_img *img, t_pixel center, int r);
void				render_polygon(t_doom *doom, t_polygon *poly);


t_bool				obj_test(t_doom *doom);

void				render_obj(t_doom *doom, t_obj *obj);
t_bool				get_obb_collision(t_obb_box a, t_obb_box b);
void				load_all(t_doom *doom);
t_bool				post_process_polygons(t_doom *doom);




t_bool				init_openal(t_doom *doom);
void				load_wav();
void				quit_openal();

#endif
