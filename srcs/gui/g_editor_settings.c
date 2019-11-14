/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/14 05:19:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "doom.h"
#include "editor.h"
#include "gui.h"

t_ressource		*get_next_texture(t_ressource_manager *r, t_ressource *res)
{
	int			i;
	t_ressource	*t_r;

	i = 0;
	if (res)
	{
		res->used--;
		i = ressources_indexof(r->ressources, res) + 1;
	}
	t_r = NULL;
	while (!t_r || t_r->type != RESSOURCE_TEXTURE || !t_r->loaded)
	{
		if (i == r->ressources->len)
			i = 0;
		t_r = r->ressources->values[i++];
	}
	if (t_r)
		t_r->used++;
	return (t_r);
}

t_ressource		*get_prev_texture(t_ressource_manager *r, t_ressource *res)
{
	int			i;
	t_ressource	*t_r;

	i = 0;
	if (res)
	{
		res->used--;
		i = ressources_indexof(r->ressources, res) - 1;
	}
	t_r = NULL;
	while (!t_r || t_r->type != RESSOURCE_TEXTURE || !t_r->loaded)
	{
		if (i == -1)
			i = r->ressources->len - 1;
		t_r = r->ressources->values[i--];
	}
	if (t_r)
		t_r->used++;
	return (t_r);
}

t_wall			*get_current_wall(t_editor *editor)
{
	if (editor->current_room == -1)
		return (NULL);
	t_room	*room = &editor->rooms->values[editor->current_room];
	int wall_index = wall_indexof_by_indice(room->walls, editor->current_seg.x);
	if (wall_index == -1)
		return (NULL);
	return (&room->walls->values[wall_index]);
}

static void		action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[0])
	{
		t_wall *wall = get_current_wall(&doom->editor);
		wall->texture = get_prev_texture(&doom->res_manager, wall->texture);
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[1])
	{
		t_wall *wall = get_current_wall(&doom->editor);
		wall->texture = get_next_texture(&doom->res_manager, wall->texture);
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[2])
	{
		t_wall *wall = get_current_wall(&doom->editor);
		wall->collisions = ((t_checkbox *)cmp)->value;
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[3])
	{
		t_wall *wall = get_current_wall(&doom->editor);
		wall->invisible = ((t_checkbox *)cmp)->value;
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[4])
	{
		t_room	*room = &doom->editor.rooms->values[doom->editor.current_room];
		room->floor_texture = get_prev_texture(&doom->res_manager, room->floor_texture);
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[5])
	{
		t_room	*room = &doom->editor.rooms->values[doom->editor.current_room];
		room->floor_texture = get_next_texture(&doom->res_manager, room->floor_texture);
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[6])
	{
		t_room	*room = &doom->editor.rooms->values[doom->editor.current_room];
		room->ceiling_texture = get_prev_texture(&doom->res_manager, room->ceiling_texture);
	}
	if (cmp == doom->guis[GUI_EDITOR_SETTINGS].components->values[7])
	{
		t_room	*room = &doom->editor.rooms->values[doom->editor.current_room];
		room->ceiling_texture = get_next_texture(&doom->res_manager, room->ceiling_texture);
	}
}

void			g_editor_settings_on_enter(t_gui *self, t_doom *doom)
{
	printf("ENTER\n");
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 70 }, "COLLISIONS"));
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 170, y + 70 }, "INVISIBLE"));

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 40, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 40, 40, 40 }, NULL, ">"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 120, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 120, 40, 40 }, NULL, ">"));
	int i = -1;
	while (++i < ES_ROOM_COMPONENTS)
		self->components->values[i]->perform_action = action_performed;
}

void			g_editor_settings_on_leave(t_gui *self, t_doom *doom)
{
	printf("LEAVE\n");
}

void			g_editor_settings_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	
}

void			g_editor_settings_render(t_gui *self, t_doom *doom)
{
	if (!doom->editor.settings_open || !doom->editor.settings_visible)
		return ;
	fill_rect(&doom->screen, (SDL_Rect){ S_WIDTH - 335, 75, 320, 550 }, 0xFF303030);
	draw_rect(&doom->screen, (SDL_Rect){ S_WIDTH - 335, 75, 320, 550 }, 0xFFFFFFFF);


	if (doom->editor.current_seg.x != -1 && doom->editor.current_room != -1)
	{
		draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 135, 0xFFFFFFFF }, (t_pixel){ S_WIDTH - 335 + 160, 235, 0 });
		t_wall	*wall = get_current_wall(&doom->editor);
		if (!wall)
			return ;
		if (wall->texture && wall->texture->type == RESSOURCE_TEXTURE)
		{
			t_img	*img = wall->texture->data.texture;
			apply_image_to_image(&doom->screen, img, (SDL_Rect){ 0, 0, img->width, img->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 85, 220, 40 });
			SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica,
				wall->texture->display_name, (SDL_Color){255, 0, 0, 0});
			apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
				(SDL_Rect){S_WIDTH - 335 + 50, 85, 220, 40});
			SDL_FreeSurface(text);
		}
	}
	else if (doom->editor.current_room != -1)
	{
		t_room	*curr_room = &doom->editor.rooms->values[doom->editor.current_room];

		SDL_Surface *text = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE FLOOR", (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 85, 220, 20});
		SDL_FreeSurface(text);
		text = TTF_RenderText_Blended(doom->fonts.helvetica, "TEXTURE CEILING", (SDL_Color){255, 255, 255, 0});
		apply_surface_blended(&doom->screen, text, (SDL_Rect){0, 0, text->w, text->h},
			(SDL_Rect){S_WIDTH - 335 + 50, 165, 220, 20});
		SDL_FreeSurface(text);
		if (curr_room->floor_texture && curr_room->floor_texture->type == RESSOURCE_TEXTURE)
		{
			t_img	*img_floor = curr_room->floor_texture->data.texture;
			apply_image_to_image(&doom->screen, img_floor, (SDL_Rect){ 0, 0, img_floor->width, img_floor->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 115, 220, 40 });
		}
		if (curr_room->ceiling_texture && curr_room->ceiling_texture->type == RESSOURCE_TEXTURE)
		{
			t_img	*img_ceiling = curr_room->ceiling_texture->data.texture;
			apply_image_to_image(&doom->screen, img_ceiling, (SDL_Rect){ 0, 0, img_ceiling->width, img_ceiling->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 195, 220, 40 });
		}
	}
	render_components(doom, self);
}
