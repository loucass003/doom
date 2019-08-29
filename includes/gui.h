/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:42:00 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/29 03:07:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUI_H
# define GUI_H

# include <SDL.h>
# include "libft.h"
# include <dirent.h>

typedef struct s_doom	t_doom;

typedef enum		e_component_type
{
	C_BUTTON,
	C_MENU,
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
	char			*image;
}					t_button;

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
	t_component		**components;
	int				component_count;
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
void				g_editor_on_enter(t_gui *self, t_doom *doom);
void				g_editor_on_leave(t_gui *self, t_doom *doom);
void				g_editor_render(t_gui *self, t_doom *doom);
void				g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom);
void				register_guis(t_doom *doom);
void				set_gui(t_doom *doom, int id);
void				render_components(t_doom *doom, t_gui *gui);
t_bool				in_bounds(SDL_Rect bounds, t_vec2 pos);
t_bool				alloc_components(t_gui *gui, int count);
void				free_components(t_gui *gui);
t_component	 		*create_button(SDL_Rect bounds, char *s);

void    			find_files(t_files **list, int *files_count, char *s, char *s2);
t_component			*create_menu(SDL_Rect bounds, char *s, t_doom *doom);
t_component	 		*create_icone(SDL_Rect bounds);

DIR					*open_dir(char *s);
void				close_dir(DIR *rep);
t_files				*copy_name(struct dirent *file, t_files *start, char *s2);
void				put_menu(t_component *self, t_doom *doom, t_menu *menu);
void				menu_deroulant(t_component *self, int i, t_texture *f, t_doom *doom);
void				display_comp(t_doom *doom, t_gui *self);

#endif