/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/18 17:57:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	if (cmp == editor->settings.guis[ES_GUI_OBJECT].components->values[0])
	{
		printf("CALL\n");
		t_object	*object = &editor->objects->values[editor->current_object];
		object->type = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value;

	}
	return (TRUE);
}

void			g_es_object_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ENTITY TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(10);
	self->components->values[0]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "PLAYER", .value = OBJECT_PLAYER });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ITEMSTACK", .value = OBJECT_ITEMSTACK });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "SPRITE", .value = OBJECT_SPRITE });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ENTITY", .value = OBJECT_ENTITY });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "MODEL", .value = OBJECT_MODEL });
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	((t_select *)self->components->values[0])->selected_item = object->type;

	doom->editor.settings.guis[OBJECT_PLAYER] = (t_gui){ .render = g_es_room_render, .on_enter = g_es_room_enter };
	doom->editor.settings.guis[OBJECT_ITEMSTACK] = (t_gui){ .render = g_es_object_render, .on_enter = g_es_object_enter, .on_leave = g_es_object_leave };
	doom->editor.settings.guis[OBJECT_SPRITE] = (t_gui){ .render = g_es_wall_render, .on_enter = g_es_wall_enter };
	doom->editor..guis[OBJECT_ENTITY] = (t_gui){ .render = g_es_wall_render, .on_enter = g_es_wall_enter };
	doom->editor.settings.guis[OBJECT_MODEL] = (t_gui){ .render = g_es_wall_render, .on_enter = g_es_wall_enter };
	
}

void			g_es_object_leave(t_gui *self, t_doom *doom)
{
	
}

void			g_es_object_render(t_gui *self, t_doom *doom)
{
	draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 135, 0xFFFF0000 }, (t_pixel){ S_WIDTH - 335 + 160, 235, 0 });
}
