/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 14:35:33 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/13 11:53:25 by louali           ###   ########.fr       */
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

# define ES_GUIS_COUNT (5)
# define ES_GUI_ROOM (0)
# define ES_GUI_WALL (1)
# define ES_GUI_OBJECT (2)
# define ES_GUI_GLOBAL (3)
# define ES_GUI_PLAYER (4)

# define ES_OBJ_GUIS_COUNT (6)

typedef enum		e_object_type
{
	OBJECT_NONE = -1,
	OBJECT_ITEMSTACK = 0,
	OBJECT_SPRITE = 1,
	OBJECT_ENTITY = 2,
	OBJECT_MODEL = 3,
	OBJECT_LIGHT = 4,
	OBJECT_TRANSPO = 5
}					t_object_type;

typedef enum		e_object_transform_mode
{
	OT_MODE_TRANSLATION = 0,
	OT_MODE_ROTATION = 1,
	OT_MODE_SCALE = 2
}					t_object_transform_mode;

typedef enum		e_uv_transform_mode
{
	UVT_MODE_OFFSET = 0,
	UVT_MODE_REPEAT = 1,
}					t_uv_transform_mode;

typedef struct		s_transpo
{
	int				connected;
	int				cooldown;
	t_bool			on_it;
}					t_transpo;

typedef union		u_object_of
{
	struct s_itemstack	*itemstack;
	struct s_sprite		*sprite;
	t_entity_type		entity;
	struct s_ressource	*model;
	struct s_transpo	*transpo;
	int					light_index;
}					t_object_of;

typedef struct		s_object
{
	t_object_type	type;
	t_vec3			pos;
	t_vec3			scale;
	t_vec3			rotation;
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

typedef enum		e_wall_section_type
{
	WS_WALL = 0,
	WS_DOOR = 1
}					t_wall_section_type;

typedef struct		s_wall_section
{
	t_wall_section_type	type;
	t_bool				invisible;
	t_bool				collisions;
	t_vec2				uv_repeat;
	t_vec2				uv_offset;
	struct s_ressource	*texture;
	uint8_t				normal_type;
	int					vertices_index[4];
	int					material_index;
}					t_wall_section;

typedef struct		s_gap_filler_packet
{
	float			start_a;
	float			start_b;
	t_vec4			range_a;
	t_vec4			range_b;
	t_vec4			last_range_a;
	t_vec4			last_range_b;
}					t_gap_filler_packet;

typedef struct		s_wall_sections
{
	int				len;
	int				capacity;
	t_wall_section	values[];
}					t_wall_sections;

typedef struct		s_wall
{
	int					indice;
	float				floor_height;
	float				ceiling_height;
	t_4dvertices		*start_rooms_range;
	t_4dvertices		*end_rooms_range;
	t_wall_sections		*wall_sections;
	int					materials_start;
	int					faces_start;
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
	t_vec3				floor_rot;
	t_vec3				ceil_rot;
	int					room_vertices_start;
	int					floor_start;
	int					ceilling_start;
	int					ceilling_end;
	int					walls_start;
	t_bool				floor_invisible;
	t_bool				ceil_invisible;
	t_bool				floor_collision;
	t_bool				ceil_collision;
	uint8_t				floor_normal;
	uint8_t				ceil_normal;
	uint8_t				ambiant_light;
	t_vec2				floor_uv_repeat;
	t_vec2				floor_uv_offset;
	t_vec2				ceil_uv_repeat;
	t_vec2				ceil_uv_offset;
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
	t_vec2					grid_cell;
	t_grid_grab				grid_cell_grab;
	t_vec2					close_seg;
	int						close_object;
	int						selected_tool;
	t_vec2					line_start_cell;
	t_rooms					*rooms;
	t_2dvertices			*points;
	t_objects				*objects;
	int						current_room;
	int						current_point;
	t_vec2					current_seg;
	int						current_object;
	int						selected_floor_ceil;
	t_bool					slope_mode;
	int						wall_section;
	t_bool					object_grab;
	t_editor_settings		settings;
	t_doom					*doom;
	int						map_renderable;
	int						walls_faces_start;
	t_object_transform_mode	object_transform_mode;
	t_uv_transform_mode		uv_transform_mode;
	
	t_bool					player_set;
	t_bool					player_grab;
	
	
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

t_wall_sections		*create_wall_sections_array(int capacity);
t_wall_sections		*append_wall_sections_array(t_wall_sections **arr, t_wall_section i);
t_wall_sections		*splice_wall_sections_array(t_wall_sections *arr,
						int index, int n);
t_wall_sections		*copy_wall_sections_array(t_wall_sections *src,
						t_wall_sections **dst);
int					wall_sections_indexof(t_wall_sections *arr, t_wall_section *elem);

int					get_close_room(t_editor *editor);
void				remove_room(t_editor *editor, int index);
t_bool				room_intersect(t_editor *editor, t_room *room, t_room *room2, t_bool strict);
t_vec2				room_height_range(t_room *room);
t_vec3				room_center(t_editor *editor, t_room *room);
void				remove_point(t_editor *editor, int index);
void				insert_point(t_editor *editor, t_vec2 seg, int point_index);
t_vec2				get_close_point(t_editor *editor, t_vec2 pos);
t_wall				init_wall(int indice);
t_wall_section		init_wall_section(t_editor *editor);
void				editor_grid_render(t_doom *doom, t_editor *editor);
void				editor_render_rooms(t_doom *doom, t_editor *editor);
t_bool				editor_render_wall(t_doom *doom, t_editor *editor, t_room *room, int j);
int					wall_indexof_by_indice(t_walls *walls, int indice);
void				editor_tool_point(t_editor *editor);
void				editor_tool_select(t_editor *editor);
void				editor_tool_point_move(t_editor *editor);
void				editor_tool_point_release(t_editor *editor);
void				editor_tool_room(t_editor *editor, SDL_Event *event);
void				editor_tool_objects(t_editor *editor);
void				editor_tool_player(t_editor *editor);
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
t_bool				get_room_gaps(t_editor *editor, t_room *room);
t_bool				update_rooms_gaps(t_editor *editor);
t_bool				update_wall(t_editor *editor, int room_index, int wall_index, int wall_section);
t_bool				post_process_map(t_editor *editor, t_renderable *r, t_bool replace);
t_bool				create_walls(t_editor *editor, t_renderable *r);
t_renderable		*get_map(t_editor *editor);
t_bool				create_map(t_renderable	*r, t_editor *editor);
t_vec3				world_to_editor(t_vec3 pos);
t_bool				editor_setmap(t_editor *editor);
void				editor_tool_objects_move(t_editor *editor);
void				editor_tool_player_move(t_editor *editor);
void				editor_tool_objects_release(t_editor *editor);
void				editor_tool_player_release(t_editor *editor);
t_bool				editor_render_player(t_doom *doom, t_editor *editor);
t_bool				editor_settings_guis(t_editor *editor);
void				set_object_default(t_doom *doom, t_object *object);
t_bool				floor_visibility(t_editor *editor, t_renderable *r, int room_index);
void				unselect_all(t_doom *doom);
t_bool				create_wall(t_renderable *r, t_editor *editor, int room_index, int wall_index, int wall_section);
t_bool				update_floor(t_editor *editor, int room_index, t_bool floor);
void				select_floor_ceil(t_editor *editor, int room_index, t_bool floor);
int					point_in_rooms(t_editor *editor, t_vec2 point);
void				hide_adjacent_walls(t_editor *editor, int room, int wall, t_wall_section *ws);
void				hide_doors_wall(t_editor *editor);

t_bool				create_map_points_and_floor(t_editor *editor, t_renderable *r);
t_bool				create_walls(t_editor *editor, t_renderable *r);



t_bool				add_map(t_renderable *rmap, t_editor *editor);




t_wall_section		create_simple_wall_section(t_editor *editor, t_room *room, int wall);
t_wall_section		create_last_wall_section(t_editor *editor, t_room *room, int wall, t_gap_filler_packet p);
t_wall_section		create_gap_wall_section(t_editor *editor, t_room *room, int wall, t_gap_filler_packet p);
t_wall_section		create_between_gaps_wall_section(t_editor *editor, t_room *room, int wall, t_gap_filler_packet p);


#endif
