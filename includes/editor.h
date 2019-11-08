#ifndef EDITOR_H
# define EDITOR_H

# include <SDL.h>
# include <libft.h>
# include "maths/line.h"
# include "gui.h"
# include "arrays.h"

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

typedef struct		s_wall
{
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
	t_ints			*indices;
	t_ints			*normals_type;
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
	GG_LINE
}					t_grid_grab;

typedef struct		s_editor
{
	t_vec2			grid_cell;
	t_grid_grab		grid_cell_grab;
	t_vec2			close_seg;
	int				selected_tool;
	t_vec2			line_start_cell;
	t_rooms			*rooms;
	t_2dvertices	*points;
	int				current_room;
	int				current_point;
	
	
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
	//t_save			*lines;
	//t_obj			*obj;
	t_line			line;
	t_vec2			last_mouse;
	SDL_Surface		*objet;
	SDL_Surface		*texture;
}					t_editor;


t_bool				check_same_point(t_doom *doom);

void				set_alert_message(t_doom *doom);
void				write_alert_message(t_doom *doom);

void				save_line_to_erase(t_doom *doom, int x, int y);
void				erase_all(t_doom *doom);

void				modify_all(t_doom *doom, int x, int y);
void				all_visual(t_doom *doom, int x, int y);
void				visual_line(t_doom *doom, t_line line);

t_bool    			in_the_poly(t_doom *doom, t_walls *walls, t_vec2 point);


void    			save_object(t_doom *doom, int x, int y, SDL_Surface *obj);
void				show_new_components(t_doom *doom);


void				print_all(t_doom *doom);

void	 			set_map(t_doom *doom);

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

t_bool				check_obj_legit(t_doom *doom, int x, int y);

#endif
