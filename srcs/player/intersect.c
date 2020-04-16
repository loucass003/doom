/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:36:31 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/16 18:33:48 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

void				intersect_collide_ellipsoid(t_doom *doom, t_renderable *r)
{
	t_collide_ellipsoid	ellipsoid;
	t_renderable		*sphere;

	ellipsoid = r->hitbox.data.ellipsoid;
	sphere = &doom->sphere_primitive;
	sphere->position = ellipsoid.origin;
	sphere->scale = ellipsoid.radius;
	sphere->dirty = TRUE;
	transform_renderable(sphere);
	r = sphere;
}

t_bool				aabb_intersect_world_utils(t_renderable *r, t_doom *doom,
	int j)
{
	if (!r->faces->values[j].has_collision
		&& doom->main_context.type == CTX_NORMAL)
		return (TRUE);
	if (r->faces->values[j].face_normal.y >= -1e-6)
		return (TRUE);
	return (FALSE);
}

t_bool				aabb_intersect_world(t_doom *doom, t_collide_aabb aabb)
{
	t_collision		hit;
	t_renderable	*r;
	int				i;
	int				j;

	i = -1;
	while (++i < doom->renderables->len)
	{
		r = &doom->renderables->values[i];
		if (ray_skip_renderable(r))
			continue;
		if (r->has_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
			intersect_collide_ellipsoid(doom, r);
		j = -1;
		while (++j < r->faces->len)
		{
			if (aabb_intersect_world_utils(r, doom, j))
				continue;
			hit = triangle_hit_aabb(&r->faces->values[j].\
				collidable.data.triangle, &aabb);
			if (hit.collide)
				return (TRUE);
		}
	}
	return (FALSE);
}
