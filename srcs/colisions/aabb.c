/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:20:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 04:42:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "collision.h"

t_collision		ray_hit_aabb(t_ray *ray, t_collide_aabb *collidable)
{
	t_vec3	dirfrac;
	float	tmax;
	float	tmin;
	float	t[6];

	dirfrac.x = 1.0f / ray->direction.x;
	dirfrac.y = 1.0f / ray->direction.y;
	dirfrac.z = 1.0f / ray->direction.z;
	t[1] = (collidable->min.x - ray->origin.x) * dirfrac.x;
	t[3] = (collidable->min.y - ray->origin.y) * dirfrac.y;
	t[5] = (collidable->min.z - ray->origin.z) * dirfrac.z;
	t[2] = (collidable->max.x - ray->origin.x) * dirfrac.x;
	t[4] = (collidable->max.y - ray->origin.y) * dirfrac.y;
	t[6] = (collidable->max.z - ray->origin.z) * dirfrac.z;
	tmax = fmin(fmin(fmax(t[0], t[1]), fmax(t[2], t[3])), fmax(t[4], t[5]));
	if (tmax < 0)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	tmin = fmax(fmax(fmin(t[0], t[1]), fmin(t[2], t[3])), fmin(t[4], t[5]));
	if (tmin > tmax)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	return ((t_collision) { .collide = TRUE, .dist = tmin,
		.who = (t_collidable){.type = COLLIDE_AABB,
		.data = { .aabb = *collidable }}});
}

t_collision				aabb_hit_aabb(t_collide_aabb *b, t_collide_aabb *a)
{
	if (a->min.x > b->max.x)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	if (a->min.y > b->max.y)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	if (a->min.z > b->max.z)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	if (a->max.x < b->min.x)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	if (a->max.y < b->min.y)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	if (a->max.z < b->min.z)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	return ((t_collision) {
		.collide = TRUE, .who = (t_collidable){
		.type = COLLIDE_AABB, .data = { .aabb = *a }}});
}
