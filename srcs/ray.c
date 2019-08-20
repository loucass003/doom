/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 16:37:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/20 14:25:29 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_ray			create_shoot_ray(t_player player, t_vec3 direction)
{
	t_vec3 d = vec3_rotate(direction, (t_vec3){-player.rotation.x, player.rotation.y, 0});
	// if (d.x == 0. && d.y == 0. && d.z == 0.)	
	// 	d = direction;
	//printf("DIR %f %f %f\n", d.x, d.y, d.x);
	return ((t_ray){
		.origin = player.pos,
		.direction = d
	});
}

t_collision		ray_hit_collidable(t_ray *ray, t_collidable *collidable)
{
	if (collidable->type == COLLIDE_AABB)
		return ray_hit_aabb(ray, &collidable->data.aabb);
	else if (collidable->type == COLLIDE_TRIANGLE)
		return ray_hit_triangle(ray, &collidable->data.triangle);
	return ((t_collision){ .collide = FALSE });
}
