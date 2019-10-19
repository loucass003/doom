/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:42:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/19 17:24:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUI_H
# define GUI_H

# include <SDL.h>
# include "libft.h"
# include <dirent.h>
# include "image.h"

typedef struct s_doom	t_doom;

typedef enum		e_component_type
{
	C_BUTTON,
	C_MENU,
	C_TEXTFIELD,
	C_PROGRESS,
	C_SELECT
}					t_component_type;

typedef struct		s_component
{
	t_component_type	type;
	SDL_Rect			bounds;
	t_bool				visible;
	t_bool				enabled;
	void				(*render)(t_doom *doom, struct s_component *self, t_img *img);
	void				(*on_click)(struct s_component *self, t_vec2 pos, t_doom *doom);
	void				(*on_mouse_move)(struct s_component *self, t_vec2 pos, t_doom *doom);
	void				(*perform_action)(struct s_component *self, t_doom *doom);
	void				(*on_event)(struct s_component *self, SDL_Event *event, t_doom *doom);
}					t_component;

typedef struct		s_components
{
	int				len;
	int				capacity;
	t_component		*values[];
}					t_components;

typedef struct		s_button
{
	t_component		super;
	int				text_len;
	char			text[256];
	char			*texte;
	int				color;
	int				color_default;
	int				color_hover;
	char			*image;
}					t_button;

typedef struct		s_progress
{
	t_component		super;
	float			value;
	int				bg_color;
	int				fg_color;
	char			text[256];
	int				text_len;
}					t_progress;

typedef struct		s_select_item
{
	char			name[255];
	ssize_t			value;
}					t_select_item;

typedef struct		s_select_items
{
	int				len;
	int				capacity;
	t_select_item	values[];
}					t_select_items;

typedef struct		s_select
{
	t_component			super;
	char				*text;
	int					selected_item;
	int					item_height;
	int					hover_item;
	t_bool				open;
	int					bg_color;
	int					hover_color;
	int					fg_color;
	t_select_items		*items;
}					t_select;

typedef struct		s_scrollbox
{
	t_component			super;
	t_img				*viewport;
	float				scroll_value;
}					t_scrollbox;

typedef struct		s_files
{
    struct s_files		*next;
    char				s[];
}					t_files;

typedef struct		s_texture
{
	SDL_Surface			*text;
	char				*name;
	int					count;
	struct s_texture	*next;
}						t_texture;

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
	t_components	*components;
	void			(*render)(struct s_gui *self, t_doom *doom);
	void			(*on_enter)(struct s_gui *self, t_doom *doom);
	void			(*on_leave)(struct s_gui *self, t_doom *doom);
	void			(*on_event)(struct s_gui *self, SDL_Event *event, t_doom *doom);
}					t_gui;

void				g_ingame_render(t_gui *self, t_doom *doom);
void				g_ingame_on_enter(t_gui *self, t_doom *doom);
void				g_mainmenu_render(t_gui *self, t_doom *doom);
void				g_mainmenu_on_enter(t_gui *self, t_doom *doom);
void				g_mainmenu_on_leave(t_gui *self, t_doom *doom);
void				g_gameover_render(t_gui *self, t_doom *doom);
void				g_gameover_on_enter(t_gui *self, t_doom *doom);
void				g_gameover_on_leave(t_gui *self, t_doom *doom);
void				g_ressources_render(t_gui *self, t_doom *doom);
void				g_ressources_on_enter(t_gui *self, t_doom *doom);
void				g_ressources_on_leave(t_gui *self, t_doom *doom);
void				g_editor_on_enter(t_gui *self, t_doom *doom);
void				g_editor_on_leave(t_gui *self, t_doom *doom);
void				g_editor_render(t_gui *self, t_doom *doom);
void				g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom);
void				register_guis(t_doom *doom);
void				set_gui(t_doom *doom, int id);
void				render_components(t_doom *doom, t_gui *gui);
t_bool				in_bounds(SDL_Rect bounds, t_vec2 pos);
// t_bool				alloc_components(t_gui *gui, int count);
void				free_components(t_gui *gui);
t_component	 		*create_button(SDL_Rect bounds, char *s, char *s2);
t_component	 		*create_progress(SDL_Rect bounds);

void    			find_files(t_files **list, int *files_count, char *s, char *s2);
t_component			*create_menu(SDL_Rect bounds, char *s, t_doom *doom);
t_component	 		*create_icone(SDL_Rect bounds);

DIR					*open_dir(char *s);
void				close_dir(DIR *rep);
t_files				*copy_name(struct dirent *file, t_files *start, char *s2);
void				put_menu(t_component *self, t_doom *doom, t_menu *menu);
void				menu_deroulant(t_component *self, int i, t_texture *f, t_doom *doom);
void				display_comp(t_doom *doom, t_gui *self);

t_select_items		*create_select_items_array(int capacity);
t_select_items		*append_select_items_array(t_select_items **arr, t_select_item v);
t_select_items		*splice_select_items_array(t_select_items *arr, int index, int n);
t_select_items		*copy_select_items_array(t_select_items *src, t_select_items **dst);
int					select_items_indexof(t_select_items *arr, t_select_item *elem);

t_components		*create_components_array(int capacity);
t_components		*append_components_array(t_components **arr, t_component *v);
t_components		*splice_components_array(t_components *arr, int index, int n);
t_components		*copy_components_array(t_components *src, t_components **dst);
int					components_indexof(t_components *arr, t_component *elem);

t_component			*create_select(SDL_Rect bounds, char *text);

#endif