/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 12:09:46 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/21 00:07:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"
#include <limits.h>
#include "collision.h"
#include "render.h"
#include "octree.h"

t_vec3			point_to_local(t_vec3 point, t_vec3 position, t_vec3 rotation,
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
