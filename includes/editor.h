/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 14:35:33 by lloncham          #+#    #+#             */
/*   Updated: 2019/12/07 18:47:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include <SDL.h>
# include <libft.h>
# include "maths/line.h"
# include "gui.h"
# include "arrays.h"
# include "entity.h"

# define TOOL_ROOM (1)
# define TOOL_POINT (2)
# define TOOL_SELECT (3)
# define TOOL_OBJECTS (4)
# define TOOL_PLAYER (5)
# define CELLS_SPACING (10)

# define ES_GUIS_COUNT (3)
# define ES_GUI_ROOM (0)
# define ES_GUI_WALL (1)
# define ES_GUI_OBJECT (2)

# define ES_OBJ_GUIS_COUNT (4)

typedef enum		e_object_type
{
	OBJECT_NONE = -1,
	OBJECT_ITEMSTACK = 0,
	OBJECT_SPRITE = 1,
	OBJECT_ENTITY = 2,
	OBJECT_MODEL = 3
}					t_object_type;

typedef union		u_object_of
{
	struct s_itemstack	*itemstack;
	struct s_sprite		*sprite;
	t_entity_type		entity;
	struct s_ressource	*model;
}					t_object_of;

typedef struct		s_object
{
	t_object_type	type;
	t_vec3			pos;
	t_vec3			scale;
	t_bool			no_light;
	t_object_of		of;
	t_renderable	*r;
}					t_object;

typedef struct		s_objects
{
	int				len;
	int				capacity;
	t_object		values[];
}					t_objects;

typedef struct		s_wall
{
	int					indice;
	struct s_ressource	*texture;
	uint8_t				normal_type;
	t_bool				invisible;
	t_bool				collisions;
	float				floor_height;
	float				ceiling_height;
}					t_wall;

typedef struct		s_walls
{
	int				len;
	int				capacity;
	t_wall			values[];
}					t_walls;

typedef struct		s_room
{
	t_walls				*walls;
	struct s_ressource	*floor_texture;
	struct s_ressource	*ceiling_texture;
	t_bool				closed;
	t_renderable		*r;
	t_vec3				floor_rot;
	t_vec3				ceil_rot;
	int					ceilling_start;
	int					walls_start;
}					t_room;

typedef struct		s_rooms
{
	int				capacity;
	int				len;
	t_room			values[];
}					t_rooms;

typedef enum		e_grid_grab
{
	GG_NONE,
	GG_POINT,
	GG_LINE,
	GG_OBJECT,
	GG_PLAYER,
	GG_OUTSIDE
}					t_grid_grab;

typedef struct		s_editor_settings
{
	t_bool			open;
	t_bool			visible;
	t_gui			guis[ES_GUIS_COUNT];
	t_gui			guis_object[ES_OBJ_GUIS_COUNT];
	int				current_gui;
	int				current_gui_object;
}					t_editor_settings;



typedef struct		s_editor
{
	t_vec2				grid_cell;
	t_grid_grab			grid_cell_grab;
	t_vec2				close_seg;
	int					close_object;
	int					selected_tool;
	t_vec2				line_start_cell;
	t_rooms				*rooms;
	t_2dvertices		*points;
	t_objects			*objects;
	int					current_room;
	int					current_point;
	t_vec2				current_seg;
	int					current_object;
	int					selected_floor_ceil;
	t_bool				object_grab;
	t_editor_settings	settings;
	t_doom				*doom;
	
	t_bool				player_set;
	t_vec3				player_pos;
	t_bool				player_grab;
	
	
	int					icone;
	SDL_Surface			*objet;
	SDL_Surface			*texture;
}					t_editor;


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
t_rooms					*copy_rooms_array(t_rooms *src,
						t_rooms **dst);

t_objects			*create_objects_array(int capacity);
t_objects			*append_objects_array(t_objects **arr, t_object i);
t_objects			*splice_objects_array(t_objects *arr,
						int index, int n);
t_objects			*copy_objects_array(t_objects *src,
						t_objects **dst);
int					objects_indexof(t_objects *arr, t_object *elem);

int					get_close_room(t_editor *editor);
void				remove_room(t_editor *editor, int index);
t_bool				room_intersect(t_editor *editor, t_room *room, t_room *room2, t_bool strict);
t_vec2				room_height_range(t_editor *editor, t_room *room);
t_vec3				room_center(t_editor *editor, t_room *room);
void				remove_point(t_editor *editor, int index);
void				insert_point(t_editor *editor, t_vec2 seg, int point_index);
t_vec2				get_close_point(t_editor *editor, t_vec2 pos);
t_wall				init_wall(t_editor *editor, int indice);
void				editor_grid_render(t_gui *self, t_doom *doom, t_editor *editor);
void				editor_render_rooms(t_gui *self, t_doom *doom, t_editor *editor);
t_bool				editor_render_wall(t_doom *doom, t_editor *editor, t_room *room, int j);
int					wall_indexof_by_indice(t_walls *walls, int indice);
void				editor_tool_point(t_editor *editor);
void				editor_tool_select(t_editor *editor, SDL_Event *event);
void				editor_tool_point_move(t_editor *editor);
void				editor_tool_point_release(t_editor *editor);
void				editor_tool_room(t_editor *editor, SDL_Event *event);
void				editor_tool_objects(t_editor *editor, SDL_Event *event);
void				editor_tool_player(t_editor *editor, SDL_Event *event);
void				editor_delete_action(t_editor *editor);
void				select_room(t_editor *editor, int index);
t_vec2				get_close_seg(t_editor *editor, t_room *room, t_vec2 pos);
void				editor_settings_update(t_editor *editor);
t_wall				*get_current_wall(t_editor *editor);
t_bool				is_settings_open(t_editor *editor);
t_bool				get_close_object(t_editor *editor, t_vec2 *pos);
int					get_object(t_editor *editor, t_vec2 pos);
t_bool				editor_render_objects(t_editor *editor);
t_bool				is_in_range(t_vec2 pos, t_vec2 test);
t_object			init_object(t_editor *editor, t_vec2 pos);
t_wall				*get_current_wall(t_editor *editor);
void				set_gui_settings(t_editor *editor, int id);
void				free_object(t_object *object);
t_bool				create_object_renderable(t_editor *editor, int object_index, t_renderable *r);
t_vec3				editor_to_world(t_vec3 pos);

#endif
