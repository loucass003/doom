/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aabb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 17:20:03 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/16 14:21:28 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_collision		ray_hit_aabb(t_ray *ray, t_collide_aabb *collidable)
{
	return ((t_collision){ .collide = FALSE, .dist = -1 });
}
