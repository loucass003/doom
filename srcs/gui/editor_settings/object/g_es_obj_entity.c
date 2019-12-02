/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_entity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/02 15:24:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "sprite.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];

	if (cmp == editor->settings.guis_object[OBJECT_ENTITY].components->values[0])
	{
		t_select_item val = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item];
		object->of.entity = val.value;
		if (object->r)
			create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
	}
	return (TRUE);
}

void			g_es_obj_entity_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;
	int			i;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ENTITY TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(1);
	self->components->values[0]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ENEMY", .value = ENTITY_ENEMY });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "BOSS", .value = ENTITY_BOSS });
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	((t_select *)self->components->values[0])->selected_item = select_items_indexof(((t_select *)self->components->values[0])->items, object->of.entity);
}

void			g_es_obj_entity_render(t_gui *self, t_doom *doom)
{

}
