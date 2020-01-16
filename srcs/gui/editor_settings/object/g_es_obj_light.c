/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:00:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/16 13:19:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;
    t_object    *object = &doom->editor.objects->values[doom->editor.current_object];

    // if (cmp == editor->settings.guis_object[OBJECT_MODEL].components->values[0])
	// {
	// 	object->of.model = get_prev_ressource(&doom->res_manager, object->of.model, RESSOURCE_MODEL);
	// }
	// if (cmp == editor->settings.guis_object[OBJECT_MODEL].components->values[1])
	// {
	// 	object->of.model = get_next_ressource(&doom->res_manager, object->of.model, RESSOURCE_MODEL);
	
	// }
	// if (object->r)
	// 	create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
	return (TRUE);
}

void			g_es_obj_light_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;
    int         i;

	append_components_array(&self->components, create_textfield((SDL_Rect){x + 10, y + 10, 300, 30}, "INTENSITY", TRUE));
	t_int_str istr = ft_int_to_str(1 * 255);
	set_text_value((t_textfield *)self->components->values[0], istr.str, istr.len);
	i = -1;
	while (++i < 1)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_obj_light_render(t_gui *self, t_doom *doom)
{
    t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	
    // draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
