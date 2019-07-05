/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:33:38 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/05 10:23:26 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_H
# define DOOM_H

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <math.h>
# include <stdlib.h>
# include <dirent.h>

# include "libft.h"
# include "constants.h"
# include "image.h"
# include "maths/line.h"
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
	t_vec2			pos;
	t_mat2			matrix;
	float			rotation;
}					t_player;

typedef enum		e_component_type
{
	C_BUTTON,
	C_MENU,
	C_TEXTFIELD
}					t_component_type;

typedef struct      s_files
{
    struct s_files   	*next;
    char                s[];
}                   t_files;

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

typedef struct		s_obj
{
	t_vec2			pos;
	SDL_Surface		*img;
	int				floor;
	struct s_obj	*next;
}					t_obj;

typedef struct		s_save
{
	int				floor;
	t_line_list		*line;
	struct s_save	*next;
}					t_save;

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
	int				set_start_pos[3];
	int				save_modif[3];
	int				floor;
	t_save			*door;
	t_save			*sector;
	t_save			*polygon;
	t_save			*lines;
	t_obj			*obj;
	t_line			line;
	t_vec2			last_mouse;
	SDL_Surface		*objet;
	SDL_Surface		*texture;
}					t_editor;

typedef struct          s_texture
{
        SDL_Surface             *text;
        char                    *name;
		int						count;
        struct s_texture        *next;
}                       t_texture;

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
	t_node			*first_node;
	uint8_t			*rendered_area;
	t_textures		textures;
	t_gui			guis[GUI_COUNT];
	int				current_gui;
	t_editor		editor;
	int				menu;
	t_line			linetodel;
	t_texture       *wall;
    t_texture       *obj;
    t_texture       *icons;
	int				open;
}					t_doom;

t_bool				init_sdl(t_doom *doom);
t_bool				sdl_quit(t_doom *doom);
void				hook_events(t_doom *doom);
void				render(t_doom *doom);

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
void	save_in_lst(t_doom *doom);

void	editor_mouse_motion(t_doom *doom, SDL_Event *event);
void	editor_mousebuttonup(t_doom *doom, int x, int y);
void	mouseonline(t_doom *doom, t_line_list *tmp, int *line);

// void	check_poly_close(t_doom *doom, t_line_list *poly);

t_bool	check_same_point(t_doom *doom);
// t_bool	check_multi_point(t_doom *doom, t_poly *poly, int x, int y);
// t_bool	check_secant_line(t_doom *doom, t_poly *poly, float x1, float x2, float y1, float y2);
// t_bool	check_multi_line(t_doom *doom, t_poly *poly, int x1, int y1, int x2, int y2);
// t_bool	check_multi_line(t_doom *doom, t_save *list, t_line line);

void	check_poly_close(t_doom *doom, t_save *liste);
void	display_comp(t_doom *doom, t_gui *self);

void	set_alert_message(t_doom *doom);
void	write_alert_message(t_doom *doom);

void    save_line_to_erase(t_doom *doom, int x, int y);
void	erase_all(t_doom *doom);

void	modify_all(t_doom *doom, int x, int y);
void	all_visual(t_doom *doom, int x, int y);
void	visual_line(t_doom *doom, t_line line);

// int		ft_listlen(t_save *poly);
t_bool     in_the_poly(t_doom *doom, t_save *lst, t_vec2 point);


void    save_object(t_doom *doom, int x, int y, SDL_Surface *obj);
void	show_new_components(t_doom *doom);

t_bool	found_intersect(t_line *line, t_line *cmp, t_line_list *pol);
t_bool	check_secant_line(t_doom *doom, t_save *list, t_line line);

void	print_all(t_doom *doom);


void    print_lst(t_doom *doom, t_save *list);

void	load_all(t_doom *doom);

#endif
