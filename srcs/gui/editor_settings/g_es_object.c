/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 12:00:43 by llelievr         ###   ########.fr       */
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

void			set_object_default(t_doom *doom, t_object *object)
{
	if (object->type == OBJECT_ITEMSTACK)
		object->of.itemstack = create_itemstack_from_type(doom, ITEM_AMMO, -1);
	else if (object->type == OBJECT_ENTITY)
		object->of.entity = ENTITY_ENEMY;
	else if (object->type == OBJECT_SPRITE)
		object->of.sprite = create_sprite((t_vec2){ 1, 1 }, get_default_texture(&doom->res_manager, TRUE));
	else if (object->type == OBJECT_MODEL)
		object->of.model = get_ressource(&doom->res_manager, RESSOURCE_MODEL);
	else if (object->type == OBJECT_LIGHT)
	{
		object->of.light_index = create_default_light(doom);
		object->no_light = TRUE;
	}
	object->scale = (t_vec3){0, 0, 0};
	object->rotation = (t_vec3){0, 0, 0};
	if (object->r)
	{
		// free_renderable(object->r, object->type == OBJECT_MODEL, FALSE);
		create_object_renderable(&doom->editor, objects_indexof(doom->editor.objects, object), object->r);
	}
}

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	if (cmp == editor->settings.guis[ES_GUI_OBJECT].components->values[0])
	{
		t_object	*object = &editor->objects->values[editor->current_object];
		if (object->type != ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value)
		{
			free_object(object);
			object->of.entity = -1;
			object->type = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value;
			if (object->type != OBJECT_NONE)
			{
				set_object_default(doom, object);
				set_es_object_gui(&doom->editor, object->type);
				((t_checkbox *)editor->settings.guis[ES_GUI_OBJECT].components->values[1])->value = object->no_light;
			}
		}
	}
	if (cmp == editor->settings.guis[ES_GUI_OBJECT].components->values[1])
	{
		t_object	*object = &editor->objects->values[editor->current_object];
		object->no_light = ((t_checkbox *)cmp)->value;
		if (object->r)
			create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
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
	// append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "PLAYER", .value = OBJECT_PLAYER });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ITEMSTACK", .value = OBJECT_ITEMSTACK });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "SPRITE", .value = OBJECT_SPRITE });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ENTITY", .value = OBJECT_ENTITY });
	if (!!get_ressource(&doom->res_manager, RESSOURCE_MODEL))
		append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "MODEL", .value = OBJECT_MODEL });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "LIGHT", .value = OBJECT_LIGHT });
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	((t_select *)self->components->values[0])->selected_item = select_items_indexof(((t_select *)self->components->values[0])->items, object->type);

	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 60}, "No Light"));
	self->components->values[1]->perform_action = action_performed;
	((t_checkbox *)self->components->values[1])->value = object->no_light;

	// doom->editor.settings.guis_object[OBJECT_PLAYER] = (t_gui){ .render = g_es_obj_player_render, .on_enter = g_es_obj_player_enter };
	doom->editor.settings.guis_object[OBJECT_ITEMSTACK] = (t_gui){ .render = g_es_obj_itemstack_render, .on_enter = g_es_obj_itemstack_enter };
	doom->editor.settings.guis_object[OBJECT_SPRITE] = (t_gui){ .render = g_es_obj_sprite_render, .on_enter = g_es_obj_sprite_enter };
	doom->editor.settings.guis_object[OBJECT_ENTITY] = (t_gui){ .render = g_es_obj_entity_render, .on_enter = g_es_obj_entity_enter };
	doom->editor.settings.guis_object[OBJECT_MODEL] = (t_gui){ .render = g_es_obj_model_render, .on_enter = g_es_obj_model_enter };
	doom->editor.settings.guis_object[OBJECT_LIGHT] = (t_gui){ .render = g_es_obj_light_render, .on_enter = g_es_obj_light_enter };

	// if (object->type != OBJECT_NONE)
		set_es_object_gui(&doom->editor, object->type);
}

void			g_es_object_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	(void)self;
	gui_events(doom, doom->editor.settings.guis_object, event, doom->editor.settings.current_gui_object);
	components_events(doom, doom->editor.settings.guis_object, event, doom->editor.settings.current_gui_object);
}

void			g_es_object_leave(t_gui *self, t_doom *doom)
{
	(void)self;
	set_es_object_gui(&doom->editor, -1);
}

void			g_es_object_render(t_gui *self, t_doom *doom)
{
	(void)self;
	//draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 135, 0xFFFF0000 }, (t_pixel){ S_WIDTH - 335 + 160, 235, 0 });
	if (doom->editor.settings.current_gui_object >= 0)
	{
		if (doom->editor.settings.guis_object[doom->editor.settings.current_gui_object].render)
			doom->editor.settings.guis_object[doom->editor.settings.current_gui_object].render(&doom->editor.settings.guis_object[doom->editor.settings.current_gui_object], doom);
		render_components(doom, &doom->editor.settings.guis_object[doom->editor.settings.current_gui_object]);
	}
}
