/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_es_obj_light.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:00:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/09 18:09:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

static void				action_textarea_light(t_textfield *tf, t_light *light)
{
	light->intensity = 255;
	if (tf->value > 255 || tf->value < 0)
		tf->error = TRUE;
	if (!tf->error)
		light->intensity = tf->value;
}

static void				action_checkbox_light(t_checkbox *cb, t_light *light,
	t_object *object, t_editor *editor)
{
	light->model_visible = cb->value;
	if (object->r)
	{
		free_renderable(object->r, FALSE, FALSE);
		create_object_renderable(editor, editor->current_object, object->r);
	}
}

static t_bool			action_performed(t_component *cmp, t_doom *doom)
{
	const t_editor	*editor = &doom->editor;
	t_object		*object;
	t_light			*light;
	t_collision		hit;

	object = &doom->editor.objects->values[doom->editor.current_object];
	light = &doom->lights->values[object->of.light_index];
	if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[0])
		light->type = ((t_select *)cmp)->items
			->values[((t_select *)cmp)->selected_item].value;
	if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[1])
		action_textarea_light((t_textfield *)cmp, light);
	if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[2])
		action_checkbox_light((t_checkbox *)cmp, light, object, &doom->editor);
	if (cmp == editor->settings.guis_object[OBJECT_LIGHT].components->values[3])
	{
		hit = ray_hit_world(doom, doom->renderables,
			create_shoot_ray(doom->player, (t_vec3){0, 0, 1}));
		if (hit.collide)
			light->dir = ft_vec3_norm(ft_vec3_sub(hit.point, light->position));
	}
	editor->settings.guis_object[OBJECT_LIGHT].components->values[3]->visible =
		light->type == LIGHT_SPOT;
	return (TRUE);
}

static void				add_light_cmps(t_gui *self, int x, int y,
	t_light *light)
{
	t_components		*cmps;

	cmps = self->components;
	append_components_array(&cmps,
		create_select((SDL_Rect){x + 10, y + 10, 300, 30}, "ENTITY TYPE"));
	((t_select *)cmps->values[0])->items = create_select_items_array(4);
	append_select_items_array(&((t_select *)cmps->values[0])->items,
		(t_select_item){ .name = "POINT", .value = LIGHT_POINT });
	append_select_items_array(&((t_select *)cmps->values[0])->items,
		(t_select_item){ .name = "SPOT", .value = LIGHT_SPOT });
	((t_select *)cmps->values[0])->selected_item =
		select_items_indexof(((t_select *)cmps->values[0])->items, light->type);
	append_components_array(&cmps,
		create_textfield((SDL_Rect){x + 10, y + 60, 300, 30}, "INTENSITY", 1));
}

void					g_es_obj_light_enter(t_gui *self, t_doom *doom)
{
	const int			x = S_WIDTH - 335;
	const int			y = 75 + 80;
	int					i;
	t_light				*light;
	t_int_str			istr;

	light = &doom->lights->values[doom->editor.
		objects->values[doom->editor.current_object].of.light_index];
	add_light_cmps(self, x, y, light);
	istr = ft_int_to_str(light->intensity);
	set_text_value((t_textfield *)self->components->values[1],
		istr.str, istr.len);
	append_components_array(&self->components,
		create_checkbox(doom, (t_vec2){ x + 10, y + 130}, "Model Visible"));
	((t_checkbox *)self->components->values[2])->value = light->model_visible;
	append_components_array(&self->components,
		create_button((SDL_Rect){x + 10, y + 160, 300, 30},
			NULL, "SET LIGHT DIRECTION"));
	self->components->values[3]->visible = light->type == LIGHT_SPOT;
	i = -1;
	while (++i < 4)
		self->components->values[i]->perform_action = action_performed;
}
