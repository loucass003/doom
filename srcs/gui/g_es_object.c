/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/17 22:55:54 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/18 00:11:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;

	// if (cmp == editor->settings.guis[ES_GUI_WALL].components->values[0])
	// {
	// 	t_wall *wall = get_current_wall(editor);
	// 	wall->texture = get_prev_texture(&doom->res_manager, wall->texture);
	// }
	
	return (TRUE);
}

void			g_es_object_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75;

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 200, 25}, "ENTITY TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(10);
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "PLAYER", .value = OBJECT_PLAYER });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ITEMSTACK", .value = OBJECT_ITEMSTACK });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "SPRITE", .value = OBJECT_SPRITE });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "ENTITY", .value = OBJECT_ENTITY });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "MODEL", .value = OBJECT_MODEL });
}

void			g_es_object_render(t_gui *self, t_doom *doom)
{
	draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 135, 0xFFFF0000 }, (t_pixel){ S_WIDTH - 335 + 160, 235, 0 });
}