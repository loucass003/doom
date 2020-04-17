/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 21:28:02 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 19:48:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"

t_vec3			ft_vec3_trim(t_vec3 v, float max_len)
{
	float len;

	len = ft_vec3_len(v);
	if (len > max_len)
	{
		len = 1.0 / len;
		v = ft_vec3_mul_s(v, len);
		v = ft_vec3_mul_s(v, max_len);
	}
	return (v);
}

t_collide_aabb	get_max_test_range(t_entity *entity, t_vec3 e_position,
	t_vec3 e_velocity)
{
	float	scale;
	t_vec3	r3_position;
	t_vec3	query_radius;

	scale = fmax(1.5, ft_vec3_len(e_velocity)) * 1.5;
	r3_position = ft_vec3_mul(e_position, entity->packet.e_radius);
	query_radius = ft_vec3_mul_s(entity->packet.e_radius, scale);
	return ((t_collide_aabb){
		.min = ft_vec3_sub(r3_position, query_radius),
		.max = ft_vec3_add(r3_position, query_radius)
	});
}

t_bool			entity_cant_slide(t_entity *entity, t_vec3 e_position,
	t_vec3 e_velocity, t_plane sliding_plane)
{
	return (entity->type != ENTITY_GRENADA
		&& entity->packet.intersect_point.y <= e_position.y
		&& sliding_plane.normal.y > 0.95 && e_velocity.y < 0);
}
