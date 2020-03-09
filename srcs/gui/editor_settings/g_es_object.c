/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 18:45:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"
#include "sprite.h"
#include "light.h"

void	 set_es_object_gui(t_editor *editor, int id)
{
	if (editor->settings.current_gui_object >= 0)
		leave_gui(editor->doom, editor->settings.guis_object, editor->settings.current_gui_object);
	editor->settings.current_gui_object = id;
	if (editor->settings.current_gui_object >= 0)
		enter_gui(editor->doom, editor->settings.guis_object, editor->settings.current_gui_object);
}

void			g_es_object_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ENTITY TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(10);
	self->components->values[0]->perform_action = g_es_action_performed;
	// append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "PLAYER", .value = OBJECT_PLAYER });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ITEMSTACK", .value = OBJECT_ITEMSTACK });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "SPRITE", .value = OBJECT_SPRITE });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ENTITY", .value = OBJECT_ENTITY });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "TRANSPO", .value = OBJECT_TRANSPO });	
	if (!!get_ressource(&doom->res_manager, RESSOURCE_MODEL))
		append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "MODEL", .value = OBJECT_MODEL });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "LIGHT", .value = OBJECT_LIGHT });
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	((t_select *)self->components->values[0])->selected_item = select_items_indexof(((t_select *)self->components->values[0])->items, object->type);

	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 60}, "No Light"));
	self->components->values[1]->perform_action = g_es_action_performed;
	((t_checkbox *)self->components->values[1])->value = object->no_light;
	doom->editor.settings.guis_object[OBJECT_ITEMSTACK] = (t_gui){ .on_enter = g_es_obj_itemstack_enter };
	doom->editor.settings.guis_object[OBJECT_SPRITE] = (t_gui){ .render = g_es_obj_sprite_render, .on_enter = g_es_obj_sprite_enter };
	doom->editor.settings.guis_object[OBJECT_ENTITY] = (t_gui){ .render = g_es_obj_entity_render, .on_enter = g_es_obj_entity_enter };
	doom->editor.settings.guis_object[OBJECT_MODEL] = (t_gui){ .render = g_es_obj_model_render, .on_enter = g_es_obj_model_enter };
	doom->editor.settings.guis_object[OBJECT_LIGHT] = (t_gui){ .on_enter = g_es_obj_light_enter };
	doom->editor.settings.guis_object[OBJECT_TRANSPO] = (t_gui){ .render = g_es_obj_transpo_render, .on_enter = g_es_obj_transpo_enter };
	set_es_object_gui(&doom->editor, object->type);
}



void			g_es_object_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	set_es_object_gui(&doom->editor, -1);
}

void			g_es_object_render(t_gui *self, t_doom *doom)
{
	(void)self;
	if (doom->editor.settings.current_gui_object >= 0)
	{
		if (doom->editor.settings.guis_object[doom->editor.settings.current_gui_object].render)
			doom->editor.settings.guis_object[doom->editor.settings.current_gui_object].render(&doom->editor.settings.guis_object[doom->editor.settings.current_gui_object], doom);
		render_components(doom, &doom->editor.settings.guis_object[doom->editor.settings.current_gui_object]);
	}
}
