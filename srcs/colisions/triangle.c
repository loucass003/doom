/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:23:29 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/14 18:01:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_collision		ray_hit_triangle(t_ray *ray, t_collide_triangle *collidable)
{
	float		t;
	float		dist;
	t_vec3		point;

	if (ft_vec3_dot(collidable->normal, ray->direction) == 0)
		return ((t_collision){ .collide = FALSE });
	dist = ft_vec3_dot(collidable->normal, collidable->points[0]);
	t = (dist - ft_vec3_dot(collidable->normal, ray->origin)) 
		/ ft_vec3_dot(collidable->normal, ray->direction);
	point = ft_vec3_add(ray->origin, ft_vec3_mul_s(ray->direction, t));
	if (ft_vec3_dot(collidable->edge_normals[0], point) > 0
		|| ft_vec3_dot(collidable->edge_normals[1], point) > 0 
		|| ft_vec3_dot(collidable->edge_normals[2], point) > 0)
		return ((t_collision){ .collide = FALSE });
	return ((t_collision){
		.collide = TRUE,
		.dist = dist
	});
}
