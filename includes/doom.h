/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/19 23:58:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

#include <pthread.h>
//#include "at_thpool.h"

# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_mixer.h>
# include <al.h>
# include <alc.h>

# include "libft.h"
# include "constants.h"
# include "image.h"
# include "obj.h"
# include "collision.h"
# include "polygon.h"
# include "light.h"
# include "editor.h"
# include "gui.h"
# include "player.h"
# include "render.h"
# include "ressource.h"

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
	SDL_Surface		*test;
	SDL_Surface		*sprite;
	SDL_Surface		*machin;
	SDL_Surface		*gun0;
	SDL_Surface		*medkit;
	SDL_Surface		*ammo1;
}					t_textures;

// typedef struct		s_ray_collide
// {
// 	t_ray			ray;
// 	t_collision		collision;
// }					t_ray_collide;

typedef struct		s_audio
{
	Mix_Chunk		*music[5];
	ALuint			buffer[5]; 
	ALuint			source[5];
	ALCdevice 		*device;
	ALCcontext 		*context;

}					t_audio;

typedef struct		s_gdata
{
	int				todo_triangles;
	int				finished_triangles;
	t_bool			all_called;
}					t_gdata;

typedef struct		s_tdata
{
	t_gdata				*gdata;
	t_render_context	*ctx;
	t_render_data		data;
}					t_tdata;


typedef struct		s_doom
{
	SDL_Window			*win;
	SDL_Renderer		*renderer;
	t_img				screen;
	t_img				screen_transparency;
	t_bool				running;
	t_stats				stats;
	t_fonts				fonts;
	t_lights			*lights;
	t_renderables		*renderables;
	// t_polygons			*polygons;
	t_render_context	main_context;
	t_renderable		sphere_primitive;
	t_player			player;
	t_vec2				mouse;
	t_textures			textures;
	t_gui				guis[GUI_COUNT];
	int					current_gui;
	t_editor			editor;
	char				*obj_working_dir;
	// t_objs				*objects;
	t_bool				collision;
	int					menu;
	t_wall				linetodel;
	t_texture			*wall;
	t_texture			*obj;
	t_texture			*icons;
	int					open;
	t_audio				audio;
	SDL_Surface 		*background;
	t_gdata				gdata;
	int					skybox_index;
	int 				indextodel;
	int					wheredel;
	t_ressource_manager	res_manager;
	//at_thpool_t			*thpool;
	//pthread_mutex_t		mutex;
	// t_ray_collide		*rays;
}					t_doom;



t_bool				init_sdl(t_doom *doom);
t_bool				sdl_quit(t_doom *doom);
void				hook_events(t_doom *doom);
void				render(t_doom *doom);
void				game_loop(t_doom *doom);

t_bool				obj_test(t_doom *doom);


void				load_all(t_doom *doom);
t_bool				post_process_map(t_doom *doom);

t_bool				init_openal(t_doom *doom);
void            	quit_openal();


#endif
