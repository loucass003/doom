/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 16:37:29 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 23:26:34 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"
#include <limits.h>
#include "collision.h"
#include "render.h"
#include "octree.h"

t_vec3	point_to_local(t_vec3 point, t_vec3 position, t_vec3 rotation,
	t_vec3 scale)
{
	t_vec3		local;

	local = ft_vec3_sub(point, position);
	local = vec3_rotate(local, ft_vec3_mul_s(rotation, -1));
	local = ft_vec3_mul(local, ft_vec3_div((t_vec3){ 1, 1, 1 }, scale));
	return (local);
}

t_ray			create_shoot_ray(t_player player, t_vec3 direction)
{
	t_vec3 d;

	d = vec3_rotate(direction, (t_vec3){-player.entity.rotation.x,
		player.entity.rotation.y, 0});
	return ((t_ray){
		.origin = player.camera.pos,
		.direction = d
	});
}

t_ray			to_local_ray(t_ray ray, t_vec3 position, t_vec3 rotation,
	t_vec3 scale)
{
	t_ray		local;

	local.doom = ray.doom;
	local.origin = point_to_local(ray.origin, position, rotation, scale);
	local.direction = vec3_rotate(ray.direction, ft_vec3_mul_s(rotation, -1));
	local.direction = ft_vec3_mul(local.direction, ft_vec3_div((t_vec3)
		{ 1, 1, 1 }, scale));
	local.direction = ft_vec3_norm(local.direction);
	return (local);
}

t_collision		to_world_collision(t_ray original_ray,
	t_collision local_collision, t_vec3 prs[3])
{
	t_collision	collision;
	t_ray		*local_ray;
	t_vec3		point;
	float		dist;

	collision = local_collision;
	local_ray = original_ray.to_local;
	point = ft_vec3_add(local_ray->origin, ft_vec3_mul_s(local_ray->direction,
		local_collision.dist));
	point = ft_vec3_mul(point, prs[2]);
	point = vec3_rotate(point, prs[1]);
	point = ft_vec3_add(point, prs[0]);
	dist = ft_vec3_len(ft_vec3_sub(original_ray.origin, point));
	collision.dist = dist;
	collision.point = point;
	return (collision);
}

t_collision		ray_hit_collidable(t_ray *ray, t_collidable *collidable)
{
	if (collidable->type == COLLIDE_AABB)
		return (ray_hit_aabb(ray, &collidable->data.aabb));
	else if (collidable->type == COLLIDE_TRIANGLE)
		return (ray_hit_triangle(ray, &collidable->data.triangle));
	return ((t_collision){ .collide = FALSE });
}

t_bool			ray_skip_renderable(t_renderable *r)
{
	return (r->of.type == RENDERABLE_ENTITY
		&& (r->of.data.entity->dead
			|| (r->of.data.entity->type == ENTITY_ROCKET)
			|| (r->of.data.entity->type == ENTITY_PLAYER)));
}

t_renderable		*ray_transform_renderable(t_doom *doom, t_renderable *r)
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
