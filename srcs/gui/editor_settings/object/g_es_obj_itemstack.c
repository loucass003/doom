/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_itemstack.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/20 16:54:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

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
		printf("NOT COOL");
		return ;
	}
	((t_select *)self->components->values[0])->selected_item = is->of->type;
	append_components_array(&self->components, create_textfield((SDL_Rect){x + 10, y + 40, 300, 30}, ));

}

void			g_es_obj_itemstack_render(t_gui *self, t_doom *doom)
{
	// draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
