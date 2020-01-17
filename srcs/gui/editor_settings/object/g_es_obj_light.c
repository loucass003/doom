/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:00:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/17 18:05:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor *editor = &doom->editor;
    t_object    *object = &doom->editor.objects->values[doom->editor.current_object];
	if (object->of.light_index == -1)
		return (TRUE);
	t_light		*light = &doom->lights->values[object->of.light_index]; 

	if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[0])
		light->type = ((t_select *)cmp)->items->values[((t_select *)cmp)->selected_item].value;
    if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[1])
	{
		light->intensity = 255;
		if (((t_textfield *)cmp)->value > 255 || ((t_textfield *)cmp)->value < 0)
			((t_textfield *)cmp)->error = TRUE;
		if (!((t_textfield *)cmp)->error)
			light->intensity = ((t_textfield *)cmp)->value;
	}
	if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[2])
	{
		light->model_visible = ((t_checkbox *)cmp)->value;
		if (object->r)
			create_object_renderable(&doom->editor, doom->editor.current_object, object->r);
	}
	return (TRUE);
}

void			g_es_obj_light_enter(t_gui *self, t_doom *doom)
{
	int			x = S_WIDTH - 335;
	int			y = 75 + 80;
    int         i;

	
	t_object    *object = &doom->editor.objects->values[doom->editor.current_object];
	if (object->of.light_index == -1)
		return ;
	t_light		*light = &doom->lights->values[object->of.light_index]; 

	append_components_array(&self->components, create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ENTITY TYPE"));
	((t_select *)self->components->values[0])->items = create_select_items_array(4);
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "POINT", .value = LIGHT_POINT });
	append_select_items_array(&((t_select *)self->components->values[0])->items, (t_select_item){ .name = "SPOT", .value = LIGHT_SPOT });
	((t_select *)self->components->values[0])->selected_item = select_items_indexof(((t_select *)self->components->values[0])->items, light->type);
	append_components_array(&self->components, create_textfield((SDL_Rect){x + 10, y + 60, 300, 30}, "INTENSITY", TRUE));
	t_int_str istr = ft_int_to_str(light->intensity);
	set_text_value((t_textfield *)self->components->values[1], istr.str, istr.len);
	append_components_array(&self->components, create_checkbox(doom, (t_vec2){ x + 10, y + 130}, "Model Visible"));
	((t_checkbox *)self->components->values[2])->value = light->model_visible;
	i = -1;
	while (++i < 3)
		self->components->values[i]->perform_action = action_performed;
}

void			g_es_obj_light_render(t_gui *self, t_doom *doom)
{
    t_object	*object = &doom->editor.objects->values[doom->editor.current_object];
	
    // draw_line(&doom->screen, (t_pixel){ S_WIDTH - 335 + 160, 235, 0xFFFFFF00 }, (t_pixel){ S_WIDTH - 335 + 160, 335, 0 });
}
