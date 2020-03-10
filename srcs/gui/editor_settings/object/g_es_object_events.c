/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_object_events.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 15:48:19 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 18:12:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "editor.h"

void			g_es_object_on_event(t_gui *self, SDL_Event *event,
	t_doom *doom)
{
	(void)self;
	gui_events(doom, doom->editor.settings.guis_object,
		event, doom->editor.settings.current_gui_object);
	components_events(doom, doom->editor.settings.guis_object,
		event, doom->editor.settings.current_gui_object);
}

static void		select_object(t_doom *doom, t_object *object, t_select *select)
{
	if (object->type != select->items->values[select->selected_item].value)
	{
		if (object->type == OBJECT_LIGHT)
			remove_light(doom, object->of.light_index);
		if (object->r)
			free_renderable(object->r, FALSE, TRUE, FALSE);
		free_object(object);
		object->type = select->items->values[select->selected_item].value;
		if (object->type != OBJECT_NONE)
		{
			set_object_default(doom, object);
			set_es_object_gui(&doom->editor, object->type);
			((t_checkbox *)doom->editor.settings.guis[ES_GUI_OBJECT]
				.components->values[1])->value = object->no_light;
		}
	}
}

t_bool			g_es_action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor	*editor = &doom->editor;
	t_object		*object;

	object = &editor->objects->values[editor->current_object];
	if (cmp == editor->settings.guis[ES_GUI_OBJECT].components->values[0])
		select_object(doom, object, (t_select *)cmp);
	if (cmp == editor->settings.guis[ES_GUI_OBJECT].components->values[1])
	{
		object->no_light = ((t_checkbox *)cmp)->value;
		if (object->r)
			object->r->no_light = object->no_light;
	}
	return (TRUE);
}

void	 set_es_object_gui(t_editor *editor, int id)
{
	if (editor->settings.current_gui_object >= 0)
		leave_gui(editor->doom, editor->settings.guis_object,
			editor->settings.current_gui_object);
	editor->settings.current_gui_object = id;
	if (editor->settings.current_gui_object >= 0)
		enter_gui(editor->doom, editor->settings.guis_object,
			editor->settings.current_gui_object);
}
