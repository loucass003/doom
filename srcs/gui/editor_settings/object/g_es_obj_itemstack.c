/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_itemstack.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 02:48:29 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	if (cmp == editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[0])
	{
		t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
		t_select_item val = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item];
		if (val.value != object->of.itemstack->of->type)
		{
			free_object(object);
			object->of.itemstack = create_itemstack_from_type(doom, (t_item_type)val.value, WEAPON_GUN);
			t_int_str istr = ft_int_to_str(object->of.itemstack->amount);
			ft_memcpy(((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text, istr.str, istr.len);
			ft_memset(((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text + istr.len, 0, 255 - istr.len);
			((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text_len = istr.len;
			editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[2]->visible = object->of.itemstack->of->type == ITEM_WEAPON;
			editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[2]->enabled = object->of.itemstack->of->type == ITEM_WEAPON;
			if (object->of.itemstack->of->type == ITEM_WEAPON)
				((t_select *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[2])->selected_item = select_items_indexof(((t_select *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[2])->items, object->of.itemstack->of->data.weapon.type);
			if (object->r)
			{
				free_renderable(object->r, FALSE, FALSE, FALSE);
				create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
			}
		}
	}
	else if (cmp == editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])
	{
		t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
		object->of.itemstack->amount = 1;
		if (!((t_textfield *)cmp)->error)
			object->of.itemstack->amount = ((t_textfield *)cmp)->value;
	}
	else if (cmp == editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[2])
	{
		t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
		t_select_item val = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item];
		if (val.value != object->of.itemstack->of->data.weapon.type)
		{
			free_object(object);
			object->of.itemstack = create_itemstack_from_type(doom, ITEM_WEAPON, (t_weapon_type)val.value);
			t_int_str istr = ft_int_to_str(object->of.itemstack->amount);
			ft_memcpy(((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text, istr.str, istr.len);
			ft_memset(((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text + istr.len, 0, 255 - istr.len);
			((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text_len = istr.len;
			if (object->r)
			{
				free_renderable(object->r, FALSE, FALSE, FALSE);
				create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
			}
		}
	}
	return (TRUE);
}

void			g_es_obj_itemstack_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ITEM TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(3);
	self->components->values[0]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "WEAPON", .value = ITEM_WEAPON });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "AMMO", .value = ITEM_AMMO });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "HEAL", .value = ITEM_HEAL });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "JETPACK", .value = ITEM_JETPACK });
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	t_itemstack	*is = object->of.itemstack;
	if (!is)
		return ;
	((t_select *)self->components->values[0])->selected_item = select_items_indexof(((t_select *)self->components->values[0])->items, is->of->type);
	append_components_array(&self->components, create_textfield((SDL_Rect){x + 10, y + 50, 300, 30}, "AMOUNT", TRUE));
	t_int_str istr = ft_int_to_str(is->amount);
	set_text_value((t_textfield *)self->components->values[1], istr.str, istr.len);
	self->components->values[1]->perform_action = action_performed;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 90, 300, 30}, "WEAPON TYPE"));
	((t_select *)self->components->values[2])->items = create_select_items_array(3);
	self->components->values[2]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[2])->items, (t_select_item){ .name = "GUN", .value = WEAPON_GUN });
	append_select_items_array(&((t_select *)self->components->values[2])->items, (t_select_item){ .name = "AXE", .value = WEAPON_AXE });
	append_select_items_array(&((t_select *)self->components->values[2])->items, (t_select_item){ .name = "GRENADA", .value = WEAPON_GRENADA });
	if (is->of->type == ITEM_WEAPON)
		((t_select *)self->components->values[2])->selected_item = select_items_indexof(((t_select *)self->components->values[2])->items, is->of->data.weapon.type);
	self->components->values[2]->visible = is->of->type == ITEM_WEAPON;
	self->components->values[2]->enabled = is->of->type == ITEM_WEAPON;
}

void			g_es_obj_itemstack_render(t_gui *self, t_doom *doom)
{
		(void)self;
		(void)doom;	
	// draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
