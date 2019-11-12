/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor_settings.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 00:11:31 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/12 20:40:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "constants.h"
#include "doom.h"
#include "editor.h"
#include "gui.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	
}

void			g_editor_settings_on_enter(t_gui *self, t_doom *doom)
{
	printf("ENTER\n");
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_button((SDL_Rect){ x + 10, y + 10, 40, 40 }, NULL, "<"));
	append_components_array(&self->components, create_button((SDL_Rect){ x + 270, y + 10, 40, 40 }, NULL, ">"));
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
		t_room	*room = &doom->editor.rooms->values[doom->editor.current_room];
		int wall_index = wall_indexof_by_indice(room->walls, doom->editor.current_seg.x);
		if (wall_index == -1)
			return ;
		t_wall	*wall = &room->walls->values[wall_index];
		apply_image_to_image(&doom->screen, wall->texture->data.texture, (SDL_Rect){ 0, 0, 200, 40 }, (SDL_Rect){ S_WIDTH - 335 + 50, 85, 260, 40 });
	}


	render_components(doom, self);
}
