/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 16:37:29 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 12:14:30 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"
#include <limits.h>
#include "collision.h"
#include "render.h"
#include "octree.h"

t_bool			ray_skip_renderable(t_renderable *r)
{
	return (r->of.type == RENDERABLE_ENTITY
		&& (r->of.data.entity->dead
			|| (r->of.data.entity->type == ENTITY_ROCKET)
			|| (r->of.data.entity->type == ENTITY_PLAYER)));
}

t_renderable	*ray_transform_renderable(t_doom *doom, t_renderable *r)
{
	t_collide_ellipsoid	ellipsoid;
	t_renderable		*sphere;

	sphere = &doom->sphere_primitive;
	ellipsoid = r->hitbox.data.ellipsoid;
	sphere->position = ellipsoid.origin;
	sphere->scale = ellipsoid.radius;
	sphere->dirty = TRUE;
	transform_renderable(sphere);
	r = sphere;
	return (r);
}

void			ray_test_faces(t_ray_test_data *data)
{
	int				j;
	t_collision		hit;
	t_vec3			tmp;

	(void)tmp;
	j = -1;
	while (++j < data->r->faces->len)
	{
		if (!data->r->faces->values[j].has_collision
			&& data->doom->main_context.type == CTX_NORMAL)
			continue;
		hit = ray_hit_collidable(data->ray->to_local,
			&data->r->faces->values[j].collidable);
		if (hit.collide)
		{
			hit = to_world_collision(*data->ray, hit, (t_vec3[3]){
				data->r->position, data->r->rotation, data->r->scale});
			if (hit.dist > 0 && hit.dist < data->min.dist)
			{
				data->min = hit;
				data->min.renderable = &data->renderables->values[data->i];
			}
		}
	}
}

t_bool			ray_test_octree(t_ray_test_data *data)
{
	t_ray		local;
	t_collision	mincpy;

	local = to_local_ray(*data->ray, data->r->position, data->r->rotation,
		data->r->scale);
	data->ray->to_local = &local;
	if (data->r->octree)
	{
		mincpy = data->min;
		ray_intersect_octree(data->r->octree, data->r, data->ray, &data->min);
		if (data->min.collide && mincpy.dist > data->min.dist)
			data->min.renderable = &data->renderables->values[data->i];
		return (FALSE);
	}
	return (TRUE);
}

t_collision		ray_hit_world(t_doom *doom, t_renderables *renderables,
	t_ray ray)
{
	t_ray_test_data	data;
	t_renderable	*r;
	int				i;

	data = (t_ray_test_data){ .i = -1, .ray = &ray, .renderables = renderables,
		.doom = doom, .min = (t_collision) { .collide = FALSE,
			.dist = INT_MAX } };
	ray.doom = doom;
	i = -1;
	while (++i < renderables->len)
	{
		r = &renderables->values[i];
		if (ray_skip_renderable(r))
			continue;
		if (r->has_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
			r = ray_transform_renderable(doom, r);
		data.r = r;
		data.i = i;
		if (!ray_test_octree(&data))
			continue;
		ray_test_faces(&data);
	}
	return (data.min);
}
