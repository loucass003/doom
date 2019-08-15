/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/15 18:35:36 by llelievr         ###   ########.fr       */
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
# include <dirent.h>

# include "libft.h"
# include "constants.h"
# include "image.h"

typedef struct		s_obb_box
{
	t_vec3			pos;
	t_vec3			x_axis;
	t_vec3			y_axis;
	t_vec3			z_axis;
	t_vec3			half_size;
}					t_obb_box;

typedef struct		s_vec4
{
	float			x;
	float			y;
	float			z;
	float			w;
}					t_vec4;

typedef	union		u_vec4_u
{
	float			a[4];
	t_vec4			v;
}					t_vec4_u;

typedef struct		s_ints
{
	int				len;
	int				capacity;
	int				values[];
}					t_ints;

typedef struct		s_4dvertices
{
	int				len;
	int				capacity;
	t_vec4			vertices[];
}					t_4dvertices;

typedef struct		s_3dvertices
{
	int				len;
	int				capacity;
	t_vec3			vertices[];
}					t_3dvertices;

typedef struct		s_2dvertices
{
	int				len;
	int				capacity;
	t_vec2			vertices[];
}					t_2dvertices;

typedef enum		s_polygon_type
{
	P_WALL,
	P_FLOOR,
	P_CEILING
}					t_polygon_type;

typedef struct		s_polygon
{
	t_polygon_type	type;
	t_3dvertices	*vertices;
	t_vec2			*uvs;
	t_ints			*indices;
	t_vec4			*pp_vertices;
	t_vec3			*normals;
	t_mat4			matrix;
	t_collidable	*collidables;
}					t_polygon;

typedef struct		s_polygons
{
	int				len;
	int				capacity;
	t_polygon		polygons[];
}					t_polygons;

typedef enum		s_side
{
	S_FRONT = 1,
	S_BACK = -1,
	S_COLINEAR = 0,
	S_SPANNING = 2
}					t_side;

typedef struct		s_line
{
	t_vec2	a;
	t_vec2	b;
	t_vec2	normal;
}					t_line;

# include "obj.h"

typedef struct s_doom t_doom;

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			direction;
}					t_ray;

typedef struct		s_collision
{
	t_bool			collide;
	float			dist;
}					t_collision;

typedef enum		e_collidable_type
{
	COLLIDE_AABB,
	COLLIDE_TRIANGLE
}					t_collidable_type;

typedef struct 		s_collidable
{
	t_collidable_type	type;
}					t_collidable;

typedef	struct		s_collide_aabb
{
	t_collidable	super;
	t_vec3			pos;
	t_vec3			size;
}					t_collide_aabb;

typedef struct		s_collide_triangle
{
	t_collidable	super;
	t_vec3			points[3];
	t_vec3			normal;
	t_vec3			edge_normals[3];
}					t_collide_triangle;

typedef struct		s_object
{
	t_vec2			pos;
	SDL_Surface		*img;
}					t_object;

typedef struct		s_objects
{
	int				len;
	int				capacity;
	t_object		values[];
}					t_objects;

// typedef enum		e_wall_type
// {
// 	W_DOOR,
// 	W_NORMAL = 0,
// 	W_TRANSPARENT
// }					t_wall_type;

typedef struct		s_wall
{
	// t_wall_type		type;
	t_line			line;
	SDL_Surface		*img;
}					t_wall;

typedef struct		s_walls
{
	int				len;
	int				capacity;
	t_wall			values[];
}					t_walls;

typedef struct		s_room
{
	t_walls			*walls;
}					t_room;

typedef struct		s_rooms
{
	int				capacity;
	int				len;
	t_room			values[];
}					t_rooms;

typedef struct		s_light
{
	t_vec3			polision;
	t_color			color;
}					t_light;

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
	t_vec3			normal;
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
	t_obb_box		obb;
	t_bool			coliding;
}					t_player;

typedef enum		e_component_type
{
	C_BUTTON,
	C_MENU,
	C_TEXTFIELD
}					t_component_type;

typedef struct		s_files
{
    struct s_files		*next;
    char				s[];
}					t_files;

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
	char			*image;
}					t_button;

typedef struct		s_menu
{
	t_component		super;
	t_bool			open;
	t_files			*name_file;
	// char			*select_file;
	SDL_Surface		*select_file;
	int				select_pos;
	int				posx;
	int				posy;
	int				files_count;
	SDL_Surface		*texture;
	int				color;
	int				click;
	char			*name;
}					t_menu;

typedef struct		s_icone
{
	t_component		super;
	t_bool			open;
	t_files			*name_file;
	char			*select_file;
	int				select_pos;
	int				posx;
	int				posy;
	int				files_count;
	SDL_Surface		*texture;
}					t_icone;

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
	uint8_t			*point;
	int				click;
	int				sup;
	int				set_start;
	int				poly;
	int				lignes;
	int				curseur;
	int				icone;
	int				porte;
	int				secteur;
	int				alert[4];
	int				set_sup[4];
	int				set_start_pos[2];
	int				save_modif[3];
	t_walls			*polygon;
	t_walls			*lines;
	t_objects		*objects;
	// t_save			*door;
	// t_save			*sector;
	// t_save			*polygon;
	// t_save			*lines;
	//t_obj			*obj;
	t_line			line;
	t_vec2			last_mouse;
	SDL_Surface		*objet;
	SDL_Surface		*texture;
}					t_editor;

typedef struct		s_texture
{
	SDL_Surface			*text;
	char				*name;
	int					count;
	struct s_texture	*next;
}						t_texture;

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
t_vec4				ft_vec4_mul(t_vec4 a, t_vec4 b);
t_vec4				ft_vec4_sub_s(t_vec4 a, float v);
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
t_component	 		*create_button(SDL_Rect bounds, char *s);

void    			find_files(t_files **list, int *files_count, char *s, char *s2);
t_component			*create_menu(SDL_Rect bounds, char *s, t_doom *doom);
t_component	 		*create_icone(SDL_Rect bounds);
t_files				*copy_name(struct dirent *file, t_files *start, char *s2);
DIR					*open_dir(char *s);
void				close_dir(DIR *rep);
void				put_menu(t_component *self, t_doom *doom, t_menu *menu);
void				menu_deroulant(t_component *self, int i, t_texture *f, t_doom *doom);

//////////EDITOR//////////////
// void	save_in_lst(t_doom *doom);

// void	editor_mouse_motion(t_doom *doom, SDL_Event *event);
// void	editor_mousebuttonup(t_doom *doom, int x, int y);
// void	mouseonline(t_doom *doom, t_line_list *tmp, int *line);

// void	check_poly_close(t_doom *doom, t_line_list *poly);

t_bool	check_same_point(t_doom *doom);
// t_bool	check_multi_point(t_doom *doom, t_poly *poly, int x, int y);
// t_bool	check_secant_line(t_doom *doom, t_poly *poly, float x1, float x2, float y1, float y2);
// t_bool	check_multi_line(t_doom *doom, t_poly *poly, int x1, int y1, int x2, int y2);
// t_bool	check_multi_line(t_doom *doom, t_save *list, t_line line);

// void	check_poly_close(t_doom *doom, t_save *liste);
void	display_comp(t_doom *doom, t_gui *self);

void	set_alert_message(t_doom *doom);
void	write_alert_message(t_doom *doom);

void    save_line_to_erase(t_doom *doom, int x, int y);
void	erase_all(t_doom *doom);

void	modify_all(t_doom *doom, int x, int y);
void	all_visual(t_doom *doom, int x, int y);
void	visual_line(t_doom *doom, t_line line);

// int		ft_listlen(t_save *poly);
t_bool     in_the_poly(t_doom *doom, t_walls *walls, t_vec2 point);


void    save_object(t_doom *doom, int x, int y, SDL_Surface *obj);
void	show_new_components(t_doom *doom);

// t_bool	found_intersect(t_line *line, t_line *cmp, t_line_list *pol);
// t_bool	check_secant_line(t_doom *doom, t_save *list, t_line line);

void	print_all(t_doom *doom);

void	 set_map(t_doom *doom);


// void    print_lst(t_doom *doom, t_save *list);

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
t_walls				*create_walls_array(int capacity);
t_walls				*append_walls_array(t_walls **arr, t_wall i);
t_walls				*splice_walls_array(t_walls *arr,
						int index, int n);
t_walls				*copy_walls_array(t_walls *src,
						t_walls **dst);

t_rooms				*create_rooms_array(int capacity);
t_rooms				*append_rooms_array(t_rooms **arr, t_room i);
t_rooms				*splice_rooms_array(t_rooms *arr,
						int index, int n);
t_rooms				*copy_rooms_array(t_rooms *src,
						t_rooms **dst);

t_objects				*create_objects_array(int capacity);
t_objects				*append_objects_array(t_objects **arr, t_object i);
t_objects				*splice_objects_array(t_objects *arr,
						int index, int n);
t_objects				*copy_objects_array(t_objects *src,
						t_objects **dst);

t_bool				inside_triangle(t_vec3 a, t_vec3 b, t_vec3 c, t_vec3 p);
 
void				draw_triangle(t_doom *doom, t_triangle projected, t_mtl *mtl);
void				process_triangle(t_doom *doom, t_mtl *mtl, t_triangle triangle);
void				post_process_triangle(t_doom *doom, t_mtl *mtl, t_triangle triangle);

t_bool				obj_test(t_doom *doom);

char				*path_join(char *dir, char *file);

float				line_get_distance(t_line l, t_vec2 p);
t_vec2				line_normal(t_line l);

void				render_obj(t_doom *doom, t_obj *obj);
t_bool				get_obb_collision(t_obb_box a, t_obb_box b);
t_bounds3			get_polygon_bounds(t_polygon *polygon);
void				load_all(t_doom *doom);
t_bool				post_process_polygons(t_doom *doom);

t_vec3				get_polygon_normal(t_polygon *poly);

t_polygon			create_wall_polygon(t_line	line, float floor, float ceil);
t_polygon			create_polygon(t_3dvertices *vertices, t_polygon_type type);

t_bool				triangulate_polygon(t_polygon *polygon);
void				uv_mapping(t_polygon *polygon);

t_ray				create_shoot_ray(t_player player, t_vec3 direction);
t_collision			ray_hit_collidable(t_ray *ray, t_collidable *collidable);
t_collision			ray_hit_aabb(t_ray *ray, t_collide_aabb *collidable);
t_collision			ray_hit_triangle(t_ray *ray, t_collide_triangle *collidable);

#endif
