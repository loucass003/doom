/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ingame.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/06 13:59:51 by lloncham          #+#    #+#             */
/*   Updated: 2020/02/06 15:22:17 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "light.h"

void                light_render(t_doom *doom)
{
	int             i;
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

void                sphere_context_editor(t_doom *doom)
{
    t_renderable *sphere;
    
    sphere = &doom->sphere_primitive;
    sphere->position = editor_to_world(doom->player.spawn_data.position);
    sphere->position.y += doom->player.entity.radius.y;
    sphere->scale = doom->player.entity.radius;
    sphere->wireframe = TRUE;
    sphere->wireframe_color = 0xFF5C5C5C;
    sphere->dirty = TRUE;
    render_renderable(&doom->main_context, sphere);
}

void                sphere_collide_ellipsoid(t_doom *doom, t_renderable *r)
{
    t_renderable *sphere;
    t_collide_ellipsoid	*ellipsoid;

    sphere = &doom->sphere_primitive;
    ellipsoid = &r->hitbox.data.ellipsoid;
    sphere->position = ellipsoid->origin;
    sphere->scale = ellipsoid->radius;
    sphere->wireframe = TRUE;
    sphere->wireframe_color = 0xFFFF0000;
    sphere->dirty = TRUE;
    render_renderable(&doom->main_context, sphere);
}

void               skybox_activated(t_doom *doom)
{
	if (doom->skybox_index != -1)
	{
		doom->renderables->values[doom->skybox_index].visible = doom->skybox_enabled;
		doom->renderables->values[doom->skybox_index].position = doom->main_context.camera->pos;
		doom->renderables->values[doom->skybox_index].dirty = TRUE;
	}
}

void               the_ingame_render(t_doom *doom, int i)
{
    t_renderable	*r;

    i = -1;
	while (++i < doom->renderables->len)
	{
		r = doom->renderables->values + i;
		if (r->of.type == RENDERABLE_ENTITY)
		{
			r->of.data.entity->r = r;
			if (!entity_update(doom, r->of.data.entity, doom->stats.delta))
				continue;
			if (r->of.data.entity->type == ENTITY_BOSS && ft_vec3_len(ft_vec3_sub(doom->player.entity.position, r->of.data.entity->position)) <= 50)
				doom->closer_boss = r->of.data.entity;
		}
		if (r->has_hitbox)
			update_hitbox(r);
		if (!render_renderable(&doom->main_context, r))
		{
			i--;
			continue;
		}
		if (r->has_hitbox && r->show_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
			sphere_collide_ellipsoid(doom, r);
	}
}