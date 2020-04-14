/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ingame.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 13:59:51 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/14 19:01:59 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "light.h"

void			light_render(t_doom *doom)
{
	int				i;
	t_renderable	*sphere;

	i = -1;
	while (++i < doom->lights->len)
	{
		sphere = &doom->sphere_primitive;
		sphere->wireframe = TRUE;
		sphere->scale = (t_vec3){ 0.2, 0.2, 0.2 };
		sphere->dirty = TRUE;
		if (!doom->lights->values[i].model_visible)
		{
			sphere->position = doom->lights->values[i].position;
			sphere->wireframe_color = 0xFFFF0000;
			render_renderable(&doom->main_context, sphere);
		}
		if (i != 0 && doom->lights->values[i].type == LIGHT_SPOT)
		{
			sphere->position = ft_vec3_mul_s(ft_vec3_add(doom->lights
				->values[i].position, doom->lights->values[i].dir), 1);
			sphere->wireframe_color = 0xFFFFFF00;
			render_renderable(&doom->main_context, sphere);
		}
	}
}

void			sphere_context_editor(t_doom *doom)
{
	t_renderable	*sphere;

	sphere = &doom->sphere_primitive;
	sphere->position = editor_to_world(doom->player.spawn_data.position);
	sphere->position.y += doom->player.entity.radius.y;
	sphere->scale = doom->player.entity.radius;
	sphere->wireframe = TRUE;
	sphere->wireframe_color = 0xFF5C5C5C;
	sphere->dirty = TRUE;
	render_renderable(&doom->main_context, sphere);
}

void			skybox_activated(t_doom *doom)
{
	t_bool	s_enable;
	t_vec3	pos;

	s_enable = doom->skybox_enabled;
	pos = doom->main_context.camera->pos;
	if (doom->skybox_index != -1)
	{
		doom->renderables->values[doom->skybox_index].visible = s_enable;
		doom->renderables->values[doom->skybox_index].position = pos;
		doom->renderables->values[doom->skybox_index].dirty = TRUE;
	}
}
