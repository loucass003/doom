/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/17 15:14:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <math.h>
# include <stdlib.h>
# include <limits.h>

# include "libft.h"
# include "constants.h"
# include "image.h"
# include "bsp.h"
# include "obj.h"

typedef struct s_doom t_doom;

typedef struct		s_bounds3
{
	t_vec3			pos;
	t_vec3			size;
}					t_bounds3;

typedef struct		s_stats
{
	int				fps;
	double			avg_ms;
	double			delta;
}					t_stats;

typedef struct		s_vertex
{
	t_vec4			pos;
	t_vec2			tex;
}					t_vertex;

typedef struct		s_triangle
{
	t_vertex		a;
	t_vertex		b;
	t_vertex		c;
}					t_triangle;

typedef struct		s_fonts
{
	TTF_Font		*helvetica;
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
	t_vec2			rotation;
	t_node			*curr_node;
	t_obb_box		obb;
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
	t_vec2			mouse;
	float			*rendered_area;
	t_textures		textures;
	t_gui			guis[GUI_COUNT];
	int				current_gui;
	t_editor		editor;
	char			*obj_working_dir;
	t_obj			*obj;
	t_bool			collision;
}					t_doom;

t_vertex			vertex_add(t_vertex a, t_vertex b);
t_vertex			vertex_sub(t_vertex a, t_vertex b);
t_vertex			vertex_div_s(t_vertex a, float v);
t_vertex			vertex_mul_s(t_vertex a, float v);
t_vertex			vertex_interpolate(t_vertex a, t_vertex b, float v);

t_vec4				vec3_to_4(t_vec3 v);
t_vec3				vec4_to_3(t_vec4 v);
t_vec3				vec4_to_3w(t_vec4 v);
t_vec4				mat4_mulv4(t_mat4 m, t_vec4 p);
t_vec4				ft_vec4_mul_s(t_vec4 a, float v);
t_vec4				ft_vec4_div_s(t_vec4 a, float v);
t_vec4				ft_vec4_sub(t_vec4 a, t_vec4 b);
t_vec4				ft_vec4_add(t_vec4 a, t_vec4 b);
t_vec4				ft_vec4_interpolate(t_vec4 a, t_vec4 b, float alpha);

t_bool 				mat4_inverse(t_mat4 a, t_mat4 *inverse);

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
void				draw_circle(t_img *img, t_pixel center, int r);
void				render_polygon(t_doom *doom, t_polygon *poly);
t_vec3				get_plane_normal(t_vec3 p0, t_vec3 p1, t_vec3 p2);

t_2dvertices		*create_2dvertices_array(int capacity);
t_2dvertices		*append_2dvertices_array(t_2dvertices **arr, t_vec2 v);
t_2dvertices		*splice_2dvertices_array(t_2dvertices *arr,
						int index, int n);
t_2dvertices		*copy_2dvertices_array(t_2dvertices *src,
						t_2dvertices **dst);

t_3dvertices		*create_3dvertices_array(int capacity);
t_3dvertices		*append_3dvertices_array(t_3dvertices **arr, t_vec3 v);
t_3dvertices		*splice_3dvertices_array(t_3dvertices *arr,
						int index, int n);
t_3dvertices		*copy_3dvertices_array(t_3dvertices *src,
						t_3dvertices **dst);

t_4dvertices		*create_4dvertices_array(int capacity);
t_4dvertices		*append_4dvertices_array(t_4dvertices **arr, t_vec4 v);
t_4dvertices		*splice_4dvertices_array(t_4dvertices *arr,
						int index, int n);
t_4dvertices		*copy_4dvertices_array(t_4dvertices *src,
						t_4dvertices **dst);

t_polygons			*create_polygons_array(int capacity);
t_polygons			*append_polygons_array(t_polygons **arr, t_polygon v);
t_polygons			*splice_polygons_array(t_polygons *arr,
						int index, int n);
t_polygons			*copy_polygons_array(t_polygons *src,
						t_polygons **dst);

t_ints				*create_ints_array(int capacity);
t_ints				*append_ints_array(t_ints **arr, int i);
t_ints				*splice_ints_array(t_ints *arr,
						int index, int n);
t_ints				*copy_ints_array(t_ints *src,
						t_ints **dst);

void				triangulate_bsp(t_node *n);
t_bool				inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p);
 
void				draw_triangle(t_doom *doom, t_triangle triangle, t_mtl *mtl);
void				process_triangle(t_doom *doom, t_mtl *mtl, t_triangle triangle);
void				post_process_triangle(t_doom *doom, t_mtl *mtl, t_triangle triangle);

t_bool				obj_test(t_doom *doom);

char				*path_join(char *dir, char *file);

float				line_get_distance(t_line l, t_vec2 p);
t_vec2				line_normal(t_line l);

void				render_obj(t_doom *doom, t_obj *obj);
void				compute_polygon_obb(t_polygon *poly);
t_bool				get_obb_collision(t_obb_box a, t_obb_box b);
t_bounds3			get_polygon_bounds(t_polygon *polygon);
void				draw_obb(t_doom *doom, t_obb_box obb);

#endif
