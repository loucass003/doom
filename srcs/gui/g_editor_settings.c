/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/13 01:03:19 by llelievr         ###   ########.fr       */
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
}

void			g_editor_settings_on_enter(t_gui *self, t_doom *doom)
{
	printf("ENTER\n");
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
	int i = -1;
	while (++i < 2)
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
		t_wall	*wall = get_current_wall(&doom->editor);
		if (!wall)
			return ;
		if (wall->texture)
		{
			t_img	*img = wall->texture->data.texture;
			apply_image_to_image(&doom->screen, img, (SDL_Rect){ 0, 0, img->width, img->height }, (SDL_Rect){ S_WIDTH - 335 + 50, 85, 260, 40 });
		}
	}
	render_components(doom, self);
}
