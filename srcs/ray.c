/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 16:37:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/06 03:26:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"
#include "collision.h"

t_vec3	point_to_local(t_vec3 point, t_vec3 position, t_vec3 rotation, t_vec3 scale)
{
	t_vec3		local;

	local = ft_vec3_sub(point, position);
	local = vec3_rotate(local, ft_vec3_mul_s(rotation, -1));
	local = ft_vec3_mul(local, ft_vec3_div((t_vec3){ 1, 1, 1 }, scale));
	return (local);
}

t_ray			create_shoot_ray(t_player player, t_vec3 direction)
{
	t_vec3 d = vec3_rotate(direction, (t_vec3){-player.entity.rotation.x, player.entity.rotation.y, 0});
	// if (d.x == 0. && d.y == 0. && d.z == 0.)
	// 	d = direction;
	//printf("DIR %f %f %f\n", d.x, d.y, d.x);
	return ((t_ray){
		.origin = player.entity.position,
		.direction = d
	});
}

t_ray			to_local_ray(t_ray ray, t_vec3 position, t_vec3 rotation, t_vec3 scale)
{
	t_ray		local;

	local.origin = point_to_local(ray.origin, position, rotation, scale);
	local.direction = vec3_rotate(ray.direction, ft_vec3_mul_s(rotation, -1));
	local.direction = ft_vec3_mul(local.direction, ft_vec3_div((t_vec3){ 1, 1, 1 }, scale));
	local.direction = ft_vec3_norm(local.direction);
	return (local);
}

t_collision		to_world_collision(t_ray original_ray, t_collision local_collision, t_vec3 position, t_vec3 rotation, t_vec3 scale)
{
	t_collision	collision;

	collision = local_collision;
	t_ray *local_ray = original_ray.to_local;
	t_vec3 point = ft_vec3_add(local_ray->origin, ft_vec3_mul_s(local_ray->direction, local_collision.dist));
	point = ft_vec3_mul(point, scale);
	point = vec3_rotate(point, rotation);
	point = ft_vec3_add(point, position);
	float dist = ft_vec3_len(ft_vec3_sub(original_ray.origin, point));
	collision.dist = dist;
	collision.point = point;
	return (collision);
}

t_collision		ray_hit_collidable(t_ray *ray, t_collidable *collidable)
{
	if (collidable->type == COLLIDE_AABB)
		return ray_hit_aabb(ray, &collidable->data.aabb);
	else if (collidable->type == COLLIDE_TRIANGLE)
		return ray_hit_triangle(ray, &collidable->data.triangle);
	return ((t_collision){ .collide = FALSE });
}
