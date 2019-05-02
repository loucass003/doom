/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/02 00:34:02 by llelievr         ###   ########.fr       */
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
# include "bsp.h"

typedef struct s_doom t_doom;

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
	t_vec3			pos;
	t_mat4			matrix;
	t_mat4			projection;
	float			rotation;
}					t_player;

typedef enum		e_component_type
{
	C_BUTTON,
	C_TEXTFIELD
}					t_component_type;

typedef struct		s_component
{
	t_component_type	type;
	SDL_Rect			bounds;
	t_bool				visible;
	t_bool				enabled;
	void				(*render)(t_doom *doom, struct s_component *self);
	void				(*on_click)(struct s_component *self, t_vec2 pos, t_doom *doom);
	void				(*on_mouse_move)(struct s_component *self, t_vec2 pos, t_doom *doom);
	void				(*perform_action)(struct s_component *self, t_doom *doom);
}					t_component;

typedef struct		s_button
{
	t_component		super;
	int				text_len;
	char			text[256];
	int				color;
	int				color_default;
	int				color_hover;
}					t_button;

typedef struct		s_gui
{
	t_component		**components;
	int				component_count;
	void			(*render)(struct s_gui *self, t_doom *doom);
	void			(*on_enter)(struct s_gui *self, t_doom *doom);
	void			(*on_leave)(struct s_gui *self, t_doom *doom);
	void			(*on_event)(struct s_gui *self, SDL_Event *event, t_doom *doom);
}					t_gui;

typedef struct		s_editor
{
/*	uint8_t			*point;
	int				click;
	t_poly			polygon;
	t_line			line;
	t_line_list		*list;
	int				alert;*/
}					t_editor;

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
	uint8_t			*rendered_area;
	t_textures		textures;
	t_gui			guis[GUI_COUNT];
	int				current_gui;
	t_editor		editor;
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

void				g_ingame_render(t_gui *self, t_doom *doom);
void				g_ingame_on_enter(t_gui *self, t_doom *doom);
void				g_mainmenu_render(t_gui *self, t_doom *doom);
void				g_mainmenu_on_enter(t_gui *self, t_doom *doom);
void				g_mainmenu_on_leave(t_gui *self, t_doom *doom);
void				g_editor_on_enter(t_gui *self, t_doom *doom);
void				g_editor_on_leave(t_gui *self, t_doom *doom);
void				g_editor_render(t_gui *self, t_doom *doom);
void				g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom);
void				register_guis(t_doom *doom);
void				set_gui(t_doom *doom, int id);
void				render_components(t_doom *doom, t_gui *gui);
t_bool				in_bounds(SDL_Rect bounds, t_vec2 pos);
t_bool				alloc_components(t_gui *gui, int count);
t_component	 		*create_button(SDL_Rect bounds);

t_mat4				projection_matrix(t_doom *doom);
void				update_maxtrix(t_doom *doom);
void				draw_line(t_img *img, t_pixel p0, t_pixel p1);
void				render_polygon(t_doom *doom, t_polygon *poly);

#endif
