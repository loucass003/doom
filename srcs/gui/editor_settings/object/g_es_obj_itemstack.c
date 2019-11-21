/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_itemstack.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/21 05:09:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

t_itemstack		*create_itemstack_from_type(t_doom *doom, t_item_type type)
{
	t_itemstack	*it;
	t_item	*item;

	item = NULL;
	if (type == ITEM_AMMO)
		item = create_item_ammo(surface_to_image(doom, doom->textures.ammo1));
	else if (type == ITEM_HEAL)
		item = create_item_heal(surface_to_image(doom, doom->textures.medkit));
	else if (type == ITEM_WEAPON)
		item = create_item_weapon_gun(surface_to_image(doom, doom->textures.gun0), surface_to_image(doom, doom->textures.gun0));
	if (!item || !(it = create_itemstack(item, 25)))
		return (NULL);
	return (it);
}

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	if (cmp == editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[0])
	{
		t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
		t_select_item val = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item];
		if (val.value != object->of.itemstack->of->type)
		{
			object->of.itemstack = create_itemstack_from_type(doom, (t_item_type)val.value);
			t_int_str istr = ft_int_to_str(object->of.itemstack->amount);
			
			ft_memcpy(((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text, istr.str, istr.len);
			((t_textfield *)editor->settings.guis_object[OBJECT_ITEMSTACK].components->values[1])->text_len = istr.len;
		}
	}
	return (TRUE);
}

void			g_es_obj_itemstack_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ITEM TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(10);
	self->components->values[0]->perform_action = action_performed;
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "WEAPON", .value = ITEM_WEAPON });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "AMMO", .value = ITEM_AMMO });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "HEAL", .value = ITEM_HEAL });
	t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	t_itemstack	*is = object->of.itemstack;
	if (!is)
	{
		printf("NOT COOL\n");
		return ;
	}
	printf("CALL %d\n", is->of->type);
	((t_select *)self->components->values[0])->selected_item = select_items_indexof(((t_select *)self->components->values[0])->items, is->of->type);
	append_components_array(&self->components, create_textfield((SDL_Rect){x + 10, y + 50, 300, 30}, "AMOUNT"));
	t_int_str istr = ft_int_to_str(is->amount);
	ft_memcpy(((t_textfield *)self->components->values[1])->text, istr.str, istr.len);
	((t_textfield *)self->components->values[1])->text_len = istr.len;
}

void			g_es_obj_itemstack_render(t_gui *self, t_doom *doom)
{
	// draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
