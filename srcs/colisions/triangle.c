/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:23:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/16 16:47:44 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_collision		ray_hit_triangle(t_ray *ray, t_collide_triangle *collidable)
{
	float		t;
	float		dist;
	t_vec3		point;

	t_vec3 v0v1 = ft_vec3_sub(collidable->points[1],collidable->points[0]);
	t_vec3 v0v2 = ft_vec3_sub(collidable->points[2],collidable->points[0]); 
	t_vec3 pvec = ft_vec3_cross(ray->direction, v0v2); 
	float det = ft_vec3_dot(v0v1, pvec); 
	if (fabs(det) < 1e-12)
		return ((t_collision){ .collide = FALSE, .dist = -1 });

	float invDet = 1. / det; 
 
	t_vec3 tvec = ft_vec3_sub(ray->origin, collidable->points[0]);
	float u = ft_vec3_dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) 
		return ((t_collision){ .collide = FALSE, .dist = -1 });
 
   	t_vec3 qvec = ft_vec3_cross(tvec, v0v1);
    float v = ft_vec3_dot(ray->direction, qvec) * invDet;
    if (v < 0 || u + v > 1) 
		return ((t_collision){ .collide = FALSE, .dist = -1 });
    dist = ft_vec3_dot(v0v2, qvec) * invDet; 
	if (dist < 0)
		return ((t_collision){ .collide = FALSE, .dist = -1 });
	return ((t_collision){
		.collide = TRUE,
		.dist = dist
	});
}
