/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_itemstack_actions.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 19:09:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 19:10:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void					update_g_es_itemstack_textfield(t_editor *e,
	t_object *object)
{
	t_int_str			istr;

	istr = ft_int_to_str(object->of.itemstack->amount);
	ft_memcpy(((t_textfield *)e->settings.guis_object[OBJECT_ITEMSTACK]
		.components->values[1])->text, istr.str, istr.len);
	ft_memset(((t_textfield *)e->settings.guis_object[OBJECT_ITEMSTACK]
		.components->values[1])->text + istr.len, 0, 255 - istr.len);
	((t_textfield *)e->settings.guis_object[OBJECT_ITEMSTACK]
		.components->values[1])->text_len = istr.len;
}

void					action_select_item_type_update(t_object *object,
	t_editor *e, t_select *s)
{
	update_g_es_itemstack_textfield(e, object);
	e->settings.guis_object[OBJECT_ITEMSTACK].components->values[2]
		->visible = object->of.itemstack->of->type == ITEM_WEAPON;
	e->settings.guis_object[OBJECT_ITEMSTACK].components->values[2]
		->enabled = object->of.itemstack->of->type == ITEM_WEAPON;
	if (object->of.itemstack->of->type == ITEM_WEAPON)
		s->selected_item = select_items_indexof(s->items,
			object->of.itemstack->of->data.weapon.type);
}

void					action_select_item_type(t_editor *e, t_select *select)
{
	t_object			*object;
	const t_select_item val = select->items->values[select->selected_item];
	t_select			*s;

	object = &e->objects->values[e->current_object];
	s = (t_select *)e->settings.guis_object[OBJECT_ITEMSTACK]
		.components->values[2];
	if (val.value != object->of.itemstack->of->type)
	{
		free_object(object);
		object->of.itemstack = create_itemstack_from_type(e->doom,
			(t_item_type)val.value, WEAPON_GUN);
		action_select_item_type_update(object, e, s);
		if (object->r)
		{
			free_renderable(object->r, FALSE, FALSE, FALSE);
			create_object_renderable(e, e->current_object, object->r);
		}
	}
}

void					action_select_item_weapon_type(t_editor *e,
	t_object *object, t_select *select)
{
	t_select_item	val;

	val = select->items->values[select->selected_item];
	if (val.value != object->of.itemstack->of->data.weapon.type)
	{
		free_object(object);
		object->of.itemstack = create_itemstack_from_type(e->doom,
			ITEM_WEAPON, (t_weapon_type)val.value);
		update_g_es_itemstack_textfield(e, object);
		if (object->r)
		{
			free_renderable(object->r, FALSE, FALSE, FALSE);
			create_object_renderable(e, e->current_object, object->r);
		}
	}
}
