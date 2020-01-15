/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/15 03:41:30 by llelievr         ###   ########.fr       */
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
	SDL_Surface		*axe;
}					t_textures;

// typedef struct		s_ray_collide
// {
// 	t_ray			ray;
// 	t_collision		collision;
// }					t_ray_collide;

typedef struct		s_audio
{
	ALuint			buffer[10];
	ALuint			source[MAX_SOUNDS];
	t_bool			source_status[MAX_SOUNDS];
	ALCdevice 		*device;
	ALCcontext 		*context;

}					t_audio;

typedef struct		s_gdata
{
	int				todo_triangles;
	int				finished_triangles;
	t_bool			all_called;
}					t_gdata;

// typedef struct		s_tdata
// {
// 	t_gdata				*gdata;
// 	t_render_context	*ctx;
// 	t_render_data		data;
// }					t_tdata;


typedef struct		s_gameover
{
	int				kill;
	int				bullets;
	int				weapon;
	float			totaldamage;
}					t_gameover;

typedef struct		s_doom
{
	SDL_Window			*win;
	SDL_Renderer		*renderer;
	t_img				screen;
	t_img				screen_transparency;
	t_img				light_view;//TODO: DEBUG
	t_bool				running;
	t_stats				stats;
	t_fonts				fonts;
	t_lights			*lights;
	t_renderables		*renderables;
	t_render_context	main_context;
	t_renderable		sphere_primitive;
	t_renderable		grenada_primitive;
	t_player			player;
	t_textures			textures;
	t_gui				guis[GUI_COUNT];
	int					current_gui;
	t_editor			editor;
	char				*obj_working_dir;
	t_texture			*wall;
	t_texture			*obj;
	t_texture			*icons;
	t_audio				audio;
	SDL_Surface 		*background;
	//t_gdata				gdata;
	int					skybox_index;
	t_bool				skybox_enabled;
	t_ressource_manager	res_manager;
	t_gameover			gameover;
	t_entity			*closer_boss;
	t_bool				mouse_focus;
}					t_doom;

float				clamp(float min, float max, float v);

t_bool				init_sdl(t_doom *doom);
t_bool				sdl_quit(t_doom *doom);
void				hook_events(t_doom *doom);
void				render(t_doom *doom);
void				game_loop(t_doom *doom);

t_bool				obj_test(t_doom *doom);


void				load_all(t_doom *doom);

t_bool				init_openal(t_doom *doom);
void            	quit_openal();

void				entity_sound(t_entity *s, int buffer, int source, int peach);
void				player_sound(t_audio *s, int source, int buffer, float peach);

t_img				*surface_to_image(t_doom *doom, SDL_Surface *s); //TODO: NEED TO BE REMOVE 
#endif
