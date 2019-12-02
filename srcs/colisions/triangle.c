/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:23:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/02 15:33:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "libft.h"
#include "collision.h"
#include "maths/vec4.h"
#include "maths/triangle.h"

#define EPSILON 1e-6

t_collision		ray_hit_triangle(t_ray *ray, t_collide_triangle *collidable)
{
	const t_vec3 pvec = ft_vec3_cross(ray->direction, collidable->v0v2);
	const float det = ft_vec3_dot(collidable->v0v1, pvec);
	const float invDet = 1.0 / det;
	
	if (!collidable->double_sided && ft_vec3_dot(collidable->normal, ray->direction) <= 0)
		return ((t_collision) { .collide = FALSE, .dist = -1.0 });
	if (fabs(det) < 0)
		return ((t_collision) { .collide = FALSE, .dist = -1.0 });
	t_vec3 tvec = ft_vec3_sub(ray->origin, collidable->points[0]);
	float u = ft_vec3_dot(tvec, pvec) * invDet;
	if (u < 0.0 || u > 1.0)
		return ((t_collision) { .collide = FALSE, .dist = -1.0 });
	t_vec3 qvec = ft_vec3_cross(tvec, collidable->v0v1);
	float v = ft_vec3_dot(ray->direction, qvec) * invDet;
	if (v < 0.0 || u + v > 1.0)
		return ((t_collision) { .collide = FALSE, .dist = -1.0 });
	float t = ft_vec3_dot(collidable->v0v2, qvec) * invDet;
	if (t < 0)
		return ((t_collision) { .collide = FALSE, .dist = -1.0 });
	t_vec2 uv = ft_vec2_add(ft_vec2_add(ft_vec2_mul_s(collidable->uv[0], 1.0 - u - v), ft_vec2_mul_s(collidable->uv[1], u)),  ft_vec2_mul_s(collidable->uv[2], v));
	return ((t_collision) {
		.collide = TRUE,
		.dist = t,
		.uv = uv,
		.ray = *ray,
		.who = (t_collidable){
			.type = COLLIDE_TRIANGLE,
			.data = { .triangle = *collidable }
		}
	});
}

