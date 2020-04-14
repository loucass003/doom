/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ingame_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:00:26 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/15 01:50:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			sphere_collide_ellipsoid(t_doom *doom, t_renderable *r)
{
	t_renderable		*sphere;
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

t_bool			ingame_render_entity(t_doom *doom, t_renderable *r)
{
	r->of.data.entity->r = r;
	if (!entity_update(doom, r->of.data.entity, doom->stats.delta))
		return (TRUE);
	if (r->of.data.entity->type == ENTITY_BOSS
		&& ft_vec3_len(ft_vec3_sub(doom->player.entity.position,
		r->of.data.entity->position)) <= 50)
		doom->closer_boss = r->of.data.entity;
	return (FALSE);
}

void			the_ingame_render(t_doom *doom)
{
	t_renderable	*r;
	int				i;

	i = -1;
	while (++i < doom->renderables->len)
	{
		r = doom->renderables->values + i;
		if (r->of.type == RENDERABLE_ENTITY)
			if (ingame_render_entity(doom, r))
				continue;
		if (r->has_hitbox)
			update_hitbox(r);
		if (!render_renderable(&doom->main_context, r))
		{
			i--;
			continue;
		}
		if (r->has_hitbox && r->show_hitbox
			&& r->hitbox.type == COLLIDE_ELLIPSOID)
			sphere_collide_ellipsoid(doom, r);
	}
}

void			ingame_scene(t_doom *doom)
{
	int			i;

	i = -1;
	doom->main_context.image = &doom->screen;
	while (++i < S_WIDTH * S_HEIGHT)
		doom->main_context.buffer[i] = 0;
	skybox_activated(doom, i);
	threads_clear(&doom->threads);
	if (doom->main_context.type == CTX_EDITOR)
		sphere_context_editor(doom);
	doom->closer_boss = NULL;
	the_ingame_render(doom);
	if (doom->main_context.type == CTX_EDITOR)
		light_render(doom);
	threads_launch(&doom->threads);
	threads_wait(&doom->threads);
}
