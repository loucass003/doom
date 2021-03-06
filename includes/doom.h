/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/20 19:21:33 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

# include <pthread.h>
# include <SDL.h>
# include <SDL_ttf.h>
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
# include "audio.h"
# include "threads.h"

typedef struct			s_entity_collision_check
{
	t_entity			*entity;
	t_renderable		*r;
}						t_entity_collision_check;

typedef struct			s_stats
{
	int					fps;
	double				avg_ms;
	double				delta;
}						t_stats;

typedef struct			s_fonts
{
	TTF_Font			*helvetica;
}						t_fonts;

typedef struct			s_score
{
	char				text[256];
	int					score;
}						t_score;

typedef struct			s_gameover
{
	int					kill;
	int					bullets;
	int					weapon;
	float				totaldamage;
}						t_gameover;

typedef enum			e_difficulty
{
	D_EASY,
	D_MEDIUM,
	D_HARD
}						t_difficulty;

typedef	struct			s_level
{
	t_difficulty		difficulty;
	float				max_life;
	float				coeff_damage;
	float				coeff_regen;
	float				coeff_speed;
}						t_level;

typedef struct			s_doom
{
	SDL_Window			*win;
	SDL_Renderer		*renderer;
	t_img				screen;
	t_bool				running;
	t_stats				stats;
	t_fonts				fonts;
	t_lights			*lights;
	t_renderables		*renderables;
	t_render_context	main_context;
	t_threads			threads;
	t_renderable		sphere_primitive;
	t_renderable		grenada_primitive;
	t_player			player;
	t_gui				guis[GUI_COUNT];
	int					current_gui;
	t_editor			editor;
	char				*obj_working_dir;
	t_audio				audio;
	int					skybox_index;
	t_bool				skybox_enabled;
	t_ressource_manager	res_manager;
	t_gameover			gameover;
	t_entity			*closer_boss;
	t_bool				mouse_focus;
	t_level				level;
	t_score				scores[5];
	t_bool				help;
	t_bool				fullscreen;
	t_message			message;
	Uint32				trigger_event;
}						t_doom;

float					clamp(float min, float max, float v);
t_bool					init_sdl(t_doom *doom);
t_bool					sdl_quit(t_doom *doom);
void					hook_events(t_doom *doom);
void					render(t_doom *doom);
void					game_loop(t_doom *doom);
t_bool					obj_test(t_doom *doom);
t_img					*surface_to_image(t_doom *doom, SDL_Surface *s);
void					give_damage(t_entity *from, t_entity *to, t_doom *doom,
							float damage);
void					damage_explo(t_entity *from, t_doom *doom,
							float damage);
void					init_bsp(t_doom *doom);
void					init_lightning(t_doom *doom);
t_vec2					get_mouse_pos(t_doom *doom);
void					update_selects(t_gui *self, t_ressource_manager *rm);
void					apply_select_value(t_gui *self, t_doom *doom);
void					ingame_scene(t_doom *doom);
void					init_gameover(t_doom *doom);
void					skybox_activated(t_doom *doom);
void					sphere_context_editor(t_doom *doom);
void					draw_transforms_type(t_editor *editor);
int						exit_error(char *str, int r);

#endif
