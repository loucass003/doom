/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_itemstack.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 19:09:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"



static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	t_editor		*e;
	t_object		*object;

	e = &doom->editor;
	object = &e->objects->values[doom->editor.current_object];
	if (cmp == e->settings.guis_object[OBJECT_ITEMSTACK].components->values[0])
		action_select_item_type((t_editor *)e, (t_select *)cmp);
	else if (cmp == e->settings.guis_object[OBJECT_ITEMSTACK]
		.components->values[1])
	{
		object->of.itemstack->amount = 1;
		if (!((t_textfield *)cmp)->error)
			object->of.itemstack->amount = ((t_textfield *)cmp)->value;
	}
	else if (cmp == e->settings.guis_object[OBJECT_ITEMSTACK]
		.components->values[2])
		action_select_item_weapon_type(e, object, (t_select *)cmp);
	return (TRUE);
}

static void				create_item_type_select(t_gui *self, int x, int y,
	t_itemstack *i)
{
	t_components	*cmps;

	cmps = self->components;
	append_components_array(&cmps,
		create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ITEM TYPE"));
	((t_select *)cmps->values[0])->items = create_select_items_array(3);
	cmps->values[0]->perform_action = action_performed;
	append_select_items_array(&((t_select *)cmps->values[0])->items,
		(t_select_item){ .name = "WEAPON", .value = ITEM_WEAPON });
	append_select_items_array(&((t_select *)cmps->values[0])->items,
		(t_select_item){ .name = "AMMO", .value = ITEM_AMMO });
	append_select_items_array(&((t_select *)cmps->values[0])->items,
		(t_select_item){ .name = "HEAL", .value = ITEM_HEAL });
	append_select_items_array(&((t_select *)cmps->values[0])->items,
		(t_select_item){ .name = "JETPACK", .value = ITEM_JETPACK });
	((t_select *)cmps->values[0])->selected_item =
		select_items_indexof(((t_select *)cmps->values[0])->items, i->of->type);
}

static void				create_item_cmps(t_gui *self, int x, int y,
	t_itemstack *is)
{
	t_components	*cmps;
	t_int_str		istr;

	cmps = self->components;
	append_components_array(&cmps,
		create_textfield((SDL_Rect){x + 10, y + 50, 300, 30}, "AMOUNT", TRUE));
	istr = ft_int_to_str(is->amount);
	set_text_value((t_textfield *)cmps->values[1], istr.str, istr.len);
	cmps->values[1]->perform_action = action_performed;
	append_components_array(&cmps,
		create_select((SDL_Rect){x + 10, y + 90, 300, 30}, "WEAPON TYPE"));
	((t_select *)cmps->values[2])->items = create_select_items_array(3);
	cmps->values[2]->perform_action = action_performed;
	append_select_items_array(&((t_select *)cmps->values[2])->items,
		(t_select_item){ .name = "GUN", .value = WEAPON_GUN });
	append_select_items_array(&((t_select *)cmps->values[2])->items,
		(t_select_item){ .name = "AXE", .value = WEAPON_AXE });
	append_select_items_array(&((t_select *)cmps->values[2])->items,
		(t_select_item){ .name = "GRENADA", .value = WEAPON_GRENADA });
	if (is->of->type == ITEM_WEAPON)
		((t_select *)cmps->values[2])->selected_item =
			select_items_indexof(((t_select *)cmps->values[2])->items,
				is->of->data.weapon.type);
	cmps->values[2]->visible = is->of->type == ITEM_WEAPON;
	cmps->values[2]->enabled = is->of->type == ITEM_WEAPON;
}

void			g_es_obj_itemstack_enter(t_gui *self, t_doom *doom)
{
	const int	x = S_WIDTH - 335;
	const int	y = 75 + 80;
	t_object	*object;
	t_itemstack	*is;

	object = &doom->editor.objects->values[doom->editor.current_object];
	is = object->of.itemstack;
	if (!is)
		return ;
	create_item_type_select(self, x, y, is);
	create_item_cmps(self, x, y, is);
}
