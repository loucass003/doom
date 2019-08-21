/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:20:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/20 14:18:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_collision		ray_hit_aabb(t_ray *ray, t_collide_aabb *collidable)
{
	printf("CALL OUTCH\n");
	return ((t_collision){ .collide = FALSE, .dist = -1 });
}

t_collision		triangle_hit_aabb(t_collide_triangle *triangle,  t_collide_aabb *aabb)
{
	const t_vec3	e = ft_vec3_mul_s(ft_vec3_sub(aabb->max, aabb->min), 0.5);
	const t_vec3	center = ft_vec3_add(aabb->min, e);

	
}
