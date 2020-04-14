/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/29 01:42:00 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 01:46:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GUI_H
# define GUI_H

# define PAGE_SIZE (17)

# include <SDL.h>
# include <libft.h>
# include <dirent.h>
# include "image.h"

typedef struct s_doom			t_doom;

typedef enum					e_component_type
{
	C_BUTTON,
	C_TEXTFIELD,
	C_PROGRESS,
	C_SELECT,
	C_CHECKBOX
}								t_component_type;

typedef struct					s_component
{
	t_component_type			type;
	SDL_Rect					bounds;
	t_bool						visible;
	t_bool						enabled;
	void						(*render)(t_doom *doom,
									struct s_component *self, t_img *img);
	t_bool						(*perform_action)(struct s_component *self,
									t_doom *doom);
	t_bool						(*on_event)(struct s_component *self,
									SDL_Event *event, t_doom *doom);
}								t_component;

typedef struct					s_components
{
	int							len;
	int							capacity;
	t_component					*values[];
}								t_components;

typedef	struct					s_message_data
{
	int							textes_count;
	char						**textes;
}								t_message_data;

typedef	struct					s_message
{
	t_bool						open;
	int							counter_m;
	int							counter_q;
	struct s_action_message		*message_data;
	struct s_action_question	*question_data;
}								t_message;

typedef struct					s_button
{
	t_component					super;
	char						*texte;
	t_img						*img;
	t_vec2						cells_count;
	t_vec2						img_start;
	t_vec2						img_end;
	int							color;
	int							color_default;
	int							color_hover;
	t_bool						selected;
	t_bool						hover;
	SDL_Color					colortext;
}								t_button;

typedef struct					s_progress
{
	t_component					super;
	float						value;
	int							bg_color;
	int							fg_color;
	char						text[256];
	int							text_len;
}								t_progress;

typedef struct					s_select_item
{
	char						name[255];
	ssize_t						value;
}								t_select_item;

typedef struct					s_select_items
{
	int							len;
	int							capacity;
	t_bool						auto_free;
	t_select_item				values[];
}								t_select_items;

typedef struct					s_select
{
	t_component					super;
	char						*text;
	int							selected_item;
	int							item_height;
	int							hover_item;
	t_bool						open;
	int							bg_color;
	int							hover_color;
	int							fg_color;
	t_select_items				*items;
}								t_select;

typedef struct					s_textfield
{
	t_component					super;
	char						*placeholder;
	char						*text;
	t_bool						focus;
	t_bool						bar;
	t_bool						number;
	t_bool						error;
	int							value;
	int							text_len;
	int							bg_color;
	int							fg_color;
	float						t0;
}								t_textfield;

typedef struct					s_checkbox
{
	t_component					super;
	char						*text;
	t_bool						value;
	int							bg_color;
	int							color_default;
	int							color_hover;
	int							fg_color;
}								t_checkbox;

typedef struct					s_gui
{
	t_components				*components;
	void						(*render)(struct s_gui *self, t_doom *doom);
	void						(*on_enter)(struct s_gui *self, t_doom *doom);
	void						(*on_leave)(struct s_gui *self, t_doom *doom);
	void						(*on_event)(struct s_gui *self,
									SDL_Event *event, t_doom *doom);
}								t_gui;

void							g_ingame_render(t_gui *self, t_doom *doom);
void							g_ingame_on_enter(t_gui *self, t_doom *doom);
void							g_ingame_on_leave(t_gui *self, t_doom *doom);
void							g_ingame_on_events(t_gui *self,
									SDL_Event *event, t_doom *doom);

void							render_hud(t_doom *doom, t_gui *s);

void							g_mainmenu_render(t_gui *self, t_doom *doom);
void							g_mainmenu_on_enter(t_gui *self, t_doom *doom);
void							g_mainmenu_on_leave(t_gui *self, t_doom *doom);

void							g_gameover_render(t_gui *self, t_doom *doom);
void							g_gameover_on_enter(t_gui *self, t_doom *doom);
void							g_gameover_on_leave(t_gui *self, t_doom *doom);

void							g_editor_settings_render(t_gui *self,
									t_doom *doom);
void							g_editor_settings_on_enter(t_gui *self,
									t_doom *doom);
void							g_editor_settings_on_leave(t_gui *self,
									t_doom *doom);
void							g_editor_settings_on_event(t_gui *self,
									SDL_Event *event, t_doom *doom);

void							g_ressources_render(t_gui *self, t_doom *doom);
void							g_ressources_on_enter(t_gui *self,
									t_doom *doom);
void							g_ressources_on_leave(t_gui *self,
									t_doom *doom);
void							g_ressources_on_event(t_gui *self,
									SDL_Event *event, t_doom *doom);

void							g_editor_on_enter(t_gui *self, t_doom *doom);
void							g_editor_on_leave(t_gui *self, t_doom *doom);
void							g_editor_render(t_gui *self, t_doom *doom);
void							g_editor_on_event(t_gui *self, SDL_Event *event,
									t_doom *doom);

void							g_es_room_render(t_gui *self, t_doom *doom);
void							g_es_room_enter(t_gui *self, t_doom *doom);

void							g_es_wall_render(t_gui *self, t_doom *doom);
void							g_es_wall_enter(t_gui *self, t_doom *doom);

void							g_es_object_render(t_gui *self, t_doom *doom);
void							g_es_object_enter(t_gui *self, t_doom *doom);
void							g_es_object_leave(t_gui *self, t_doom *doom);
void							g_es_object_on_event(t_gui *self,
									SDL_Event *event, t_doom *doom);
t_bool							g_es_action_performed(t_component *cmp,
									t_doom *doom);
void							g_es_player_enter(t_gui *self, t_doom *doom);
void							g_es_player_render(t_gui *self, t_doom *doom);

void							g_es_obj_itemstack_enter(t_gui *self,
									t_doom *doom);
void							g_es_obj_itemstack_render(t_gui *self,
									t_doom *doom);
void							g_es_obj_sprite_enter(t_gui *self,
									t_doom *doom);
void							g_es_obj_sprite_render(t_gui *self,
									t_doom *doom);
void							g_es_obj_entity_enter(t_gui *self,
									t_doom *doom);
void							g_es_obj_entity_render(t_gui *self,
									t_doom *doom);
void							g_es_obj_model_enter(t_gui *self, t_doom *doom);
void							g_es_obj_model_render(t_gui *self,
									t_doom *doom);
void							g_es_obj_light_enter(t_gui *self, t_doom *doom);
void							g_es_obj_light_render(t_gui *self,
									t_doom *doom);
void							g_es_obj_transpo_render(t_gui *self,
									t_doom *doom);
void							g_es_obj_transpo_enter(t_gui *self,
									t_doom *doom);
void							g_es_global_enter(t_gui *self, t_doom *doom);
void							g_es_global_render(t_gui *self, t_doom *doom);

void							g_level_render(t_gui *self, t_doom *doom);
void							g_level_on_enter(t_gui *self, t_doom *doom);
void							g_level_on_leave(t_gui *self, t_doom *doom);

void							g_esc_render(t_gui *self, t_doom *doom);
void							g_esc_on_event(t_gui *self, SDL_Event *event,
									t_doom *doom);

void							g_leaderboard_on_enter(t_gui *self,
									t_doom *doom);
void							g_leaderboard_on_leave(t_gui *self,
									t_doom *doom);
void							g_leaderboard_render(t_gui *self, t_doom *doom);

void							g_message_on_enter(t_gui *self, t_doom *doom);
void							g_message_on_leave(t_gui *self, t_doom *doom);
void							g_message_render(t_gui *self, t_doom *doom);
void							g_message_on_event(t_gui *self,
									SDL_Event *event, t_doom *doom);

void							g_editor_button(t_gui *self, t_doom *doom);
void							editor_mousebuttonup(t_doom *doom);
void							editor_mousebuttondown(SDL_Event *event,
									t_doom *doom);
void							editor_keydown(SDL_Event *event, t_doom *doom);
void							editor_mousemotion(SDL_Event *event,
									t_doom *doom);
void							register_guis(t_doom *doom);
void							enter_gui(t_doom *doom, t_gui *guis, int id);
void							leave_gui(t_doom *doom, t_gui *guis, int id);
void							gui_events(t_doom *doom, t_gui *guis,
									SDL_Event *event, int id);
void							components_events(t_doom *doom, t_gui *guis,
									SDL_Event *event, int id);
void							set_gui(t_doom *doom, int id);
void							render_components(t_doom *doom, t_gui *gui);
t_bool							in_bounds(SDL_Rect bounds, t_vec2 pos);
void							free_components(t_gui *gui);
t_component						*create_button(SDL_Rect bounds, t_img *img,
									char *s2);
t_component						*create_progress(SDL_Rect bounds);
void							display_comp(t_doom *doom, t_gui *self);
t_select_items					*create_select_items_array(int capacity);
t_select_items					*append_select_items_array(t_select_items **arr,
									t_select_item v);
t_select_items					*splice_select_items_array(t_select_items *arr,
									int index, int n);
t_select_items					*copy_select_items_array(t_select_items *src,
									t_select_items **dst);
int								select_items_indexof(t_select_items *arr,
									int item_value);
t_components					*create_components_array(int capacity);
t_components					*append_components_array(t_components **arr,
									t_component *v);
t_components					*splice_components_array(t_components *arr,
									int index, int n);
t_components					*copy_components_array(t_components *src,
									t_components **dst);
int								components_indexof(t_components *arr,
									t_component *elem);
t_component						*create_select(SDL_Rect bounds, char *text);
t_component						*create_textfield(SDL_Rect bounds,
									char *placeholder, t_bool number);
void							c_textfield_render(t_doom *doom,
									t_component *self, t_img *image);
void							set_text_value(t_textfield *tf, char *text,
									int len);
t_component						*create_checkbox(t_doom *doom, t_vec2 pos,
									char *text);
void							apply_btn_image(t_doom *doom, t_component *self,
									t_button *btn);
void							apply_text(t_doom *doom, t_component *self,
									t_button *btn);
void							apply_textfield(t_doom *doom, char *str,
									t_component *self, t_textfield *tf);
void							set_button_cell(t_button *btn, int x, int y);

t_bool							selected_tool(t_doom *doom, int last_select);
int								total_score(t_doom *doom);
t_bool							add_score(char *text, int score, t_doom *doom);


#endif
