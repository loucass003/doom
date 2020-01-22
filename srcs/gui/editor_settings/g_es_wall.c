/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 20:40:10 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/22 13:22:18 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	t_editor *editor = &doom->editor;

	t_wall *wall = get_current_wall(editor);
	t_wall_section *ws = &wall->wall_sections->values[editor->wall_section];
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[0])
	{
		editor->wall_section--;
		if (editor->wall_section < 0)
			editor->wall_section = wall->wall_sections->len - 1;
		
	}
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[1])
	{
		editor->wall_section++;
		if (editor->wall_section >= wall->wall_sections->len)
			editor->wall_section = 0;
	}
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[2])
		ws->texture = get_prev_ressource(&doom->res_manager, ws->texture, RESSOURCE_TEXTURE);
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[3])
		ws->texture = get_next_ressource(&doom->res_manager, ws->texture, RESSOURCE_TEXTURE);
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[4])
		ws->collisions = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[5])
		ws->invisible = ((t_checkbox *)cmp)->value;
	if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[6])
		ws->normal_type = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value;
	update_wall(editor, editor->current_room, wall_indexof_by_indice(editor->rooms->values[editor->current_room].walls, editor->current_seg.x), doom->editor.wall_section);
	editor_settings_update(&doom->editor);
	return (TRUE);
}

void			g_es_wall_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	t_wall *wall = get_current_wall(&doom->editor);
	if (!wall || doom->editor.wall_section == -1)
		return ;
	t_wall_section *ws = &wall->wall_sections->values[doom->editor.wall_section];
	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 70, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 70, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 130 }, "COLLISIONS"));
	((t_checkbox *)self->components->values[4])->value = ws->collisions;
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 170, y + 130 }, "INVISIBLE"));
	((t_checkbox *)self->components->values[5])->value = ws->invisible;
	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 150, 300, 30}, "NORMAL TYPE"));
	((t_select *)self->components->values[6])->items = create_select_items_array(3);
	self->components->values[6]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[6])->items, (t_select_item){ .name = "FRONT", .value = 0 });
	append_select_items_array(&((t_select *)self->components->values[6])->items, (t_select_item){ .name = "BACK", .value = 1 });
	append_select_items_array(&((t_select *)self->components->values[6])->items, (t_select_item){ .name = "DOUBLE SIDED", .value = 2 });
	((t_select *)self->components->values[6])->selected_item = select_items_indexof(((t_select *)self->components->values[6])->items, ws->normal_type);
	int i = -1;
	while (++i < 7)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_wall_render(t_gui *self, t_doom *doom)
{
	(void)self;
	draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 195, 0xFFFFFFFF }, (t_pixel){ S_WIDTH - 335 + 160, 215, 0 });
	t_wall	*wall = get_current_wall(&doom->editor);
	if (!wall || doom->editor.wall_section == -1)
		return ;
	t_wall_section *ws = &wall->wall_sections->values[doom->editor.wall_section];
	SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica, "Section :", (SDL_Color){255, 0, 0, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50, 85, 180, 40});
	SDL_FreeSurface(text);
	text = TTF_RenderText_Blended(doom->fonts.helvetica, ft_int_to_str(doom->editor.wall_section).str, (SDL_Color){255, 0, 0, 0});
	apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
		(SDL_Rect){S_WIDTH - 335 + 50 + 180, 85, 40, 40});
	SDL_FreeSurface(text);
	if (ws->texture && ws->texture->type == RESSOURCE_TEXTURE)
	{
		t_img	*img = ws->texture->data.texture;
		apply_image_to_image(&doom->screen, img, (SDL_Rect){ 0, 0, img->width, img->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 145, 220, 40 });
		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
			ws->texture->display_name, (SDL_Color){255, 0, 0, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 145, 220, 40});
		SDL_FreeSurface(text);
	}
}
