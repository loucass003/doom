/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ingame.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 13:59:51 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/15 01:44:58 by llelievr         ###   ########.fr       */
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

void			render_hud(t_doom *doom, t_gui *s)
{
	doom->main_context.image->pixels[(doom->main_context.image->height / 2)
		* doom->main_context.image->width
		+ doom->main_context.image->width / 2] = 0xFF00FF00;
	draw_circle(doom->main_context.image,
		(t_pixel){ S_WIDTH_2, S_HEIGHT_2, 0xFF00FF00 }, 10);
	doom->main_context.image = &doom->screen;
	if (!draw_player_inventory(doom, s))
		return ;
	draw_transforms_type(&doom->editor);
	s->components->values[0]->visible = doom->main_context.type == CTX_NORMAL;
	s->components->values[1]->visible = doom->main_context.type == CTX_NORMAL
		&& !!doom->closer_boss;
	if (doom->closer_boss)
		((t_progress *)s->components->values[1])->value = doom->closer_boss
			->life * (1 / doom->closer_boss->max_life) * 100;
	render_components(doom, s);
}
