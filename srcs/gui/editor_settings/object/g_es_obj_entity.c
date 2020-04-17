/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_entity.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/10 17:58:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "sprite.h"
#include "doom.h"

static t_bool	action_performed(t_component *cmp, t_doom *doom)
{
	t_editor		*e;
	t_object		*object;
	t_select_item	val;

	e = &doom->editor;
	object = &doom->editor.objects->values[doom->editor.current_object];
	if (cmp == e->settings.guis_object[OBJECT_ENTITY].components->values[0])
	{
		val = ((t_select *)cmp)->items
			->values[((t_select *)cmp)->selected_item];
		object->of.entity = val.value;
		if (object->r)
		{
			free_renderable(object->r, FALSE, FALSE);
			create_object_renderable(e, e->current_object, object->r);
		}
	}
	return (TRUE);
}

static t_bool	is_boss_in_map(t_doom *doom)
{
	int				i;
	t_object		o;

	i = -1;
	while (++i < doom->editor.objects->len)
	{
		o = doom->editor.objects->values[i];
		if (o.type == OBJECT_ENTITY && o.of.entity == ENTITY_BOSS)
			return (TRUE);
	}
	return (FALSE);
}

void			g_es_obj_entity_enter(t_gui *self, t_doom *doom)
{
	const int			x = S_WIDTH - 335;
	const int			y = 75 + 80;
	t_object			*object;

	object = &doom->editor.objects->values[doom->editor.current_object];
	append_components_array(&self->components,
		create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ENTITY TYPE"));
	((t_select *)self->components->values[0])->items =
		create_select_items_array(1);
	self->components->values[0]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[0])->items,
		(t_select_item){ .name = "ENEMY", .value = ENTITY_ENEMY });
	if (!is_boss_in_map(doom) || object->of.entity == ENTITY_BOSS)
		append_select_items_array(&((t_select *)self->components->values[0])
			->items, (t_select_item){ .name = "BOSS", .value = ENTITY_BOSS });
	((t_select *)self->components->values[0])->selected_item =
		select_items_indexof(((t_select *)self->components->values[0])->items,
			object->of.entity);
}

void			g_es_obj_entity_render(t_gui *self, t_doom *doom)
{
	(void)self;
	(void)doom;
}
