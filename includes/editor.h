/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/08 14:35:33 by lloncham          #+#    #+#             */
/*   Updated: 2019/11/11 18:03:41 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H

# include <SDL.h>
# include <libft.h>
# include "maths/line.h"
# include "gui.h"
# include "arrays.h"

# define TOOL_ROOM (1)
# define TOOL_POINT (2)
# define TOOL_SELECT (3)
# define CELLS_SPACING (10)

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

// typedef struct		s_wall
// {
// 	t_line			line;
// 	SDL_Surface		*img;
// }					t_wall;

typedef struct		s_wall
{
	int				indice;
	uint8_t			normal_type;
	t_bool			invisible;
	t_bool			collisions;
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
	// t_ints			*indices;
	// t_ints			*normals_type;
	t_bool			closed;
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
	GG_OUTSIDE
}					t_grid_grab;

typedef struct		s_editor
{
	t_vec2			grid_cell;
	t_grid_grab		grid_cell_grab;
	t_vec2			close_seg;
	t_vec2			current_seg;
	int				selected_tool;
	t_vec2			line_start_cell;
	t_rooms			*rooms;
	t_2dvertices	*points;
	int				current_room;
	int				current_point;
	t_bool			settings_open;
	t_bool			settings_visible;
	
	
	// uint8_t			*point;
	// int				click;
	// int				sup;
	// int				set_start;
	// int				poly;
	// int				lignes;
	// int				curseur;
	int				icone;
	// int				porte;
	// int				secteur;
	// int				alert[4];
	// int				set_sup[4];
	// int				set_start_pos[2];
	// int				save_modif[3];
	// t_walls			*polygon;
	// t_walls			*lines;
	// t_objects		*objects;
	// // t_save			*door;
	// // t_save			*sector;
	// // t_save			*polygon;
	// //t_save			*lines;
	// //t_obj			*obj;
	// t_line			line;
	// t_vec2			last_mouse;
	SDL_Surface		*objet;
	SDL_Surface		*texture;
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

int					get_close_room(t_editor *editor);
void				remove_room(t_editor *editor, int index);
t_bool				room_intersect(t_editor *editor, t_room *room, t_room *room2, t_bool strict);
void				remove_point(t_editor *editor, int index);
void				insert_point(t_editor *editor, t_vec2 seg, int point_index);
t_vec2				get_close_point(t_editor *editor, t_vec2 pos);
t_wall				init_wall(int indice);
void				editor_grid_render(t_gui *self, t_doom *doom, t_editor *editor);
void				editor_render_rooms(t_gui *self, t_doom *doom, t_editor *editor);
t_bool				editor_render_wall(t_doom *doom, t_editor *editor, t_room *room, int j);
int					wall_indexof_by_indice(t_walls *walls, int indice);
void				editor_tool_point(t_editor *editor);
void				editor_tool_select(t_editor *editor, SDL_Event *event);
void				editor_tool_point_move(t_editor *editor);
void				editor_tool_point_release(t_editor *editor);
void				editor_tool_room(t_editor *editor, SDL_Event *event);
void				editor_delete_action(t_editor *editor);
void				select_room(t_editor *editor, int index);
t_vec2				get_close_seg(t_editor *editor, t_room *room, t_vec2 pos);
void				editor_settings_update(t_editor *editor);

#endif
