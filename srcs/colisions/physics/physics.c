/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 23:50:58 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 21:53:46 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <limits.h>
#include "maths/triangle.h"
#include "maths/plane.h"
#include "doom.h"
#include "collision.h"

t_bool				get_delays(t_physics_fields *fields, t_physics_data *packet)
{
	float	dist_to_plane;
	float	normal_dot_velocity;

	dist_to_plane = distance_to_plane(fields->plane, packet->e_base_point);
	normal_dot_velocity = ft_vec3_dot(fields->plane.normal, packet->e_velocity);
	if (normal_dot_velocity == 0)
	{
		if (fabsf(dist_to_plane) >= 1)
			return (FALSE);
		fields->t0 = 0;
		fields->t1 = 1;
		fields->in_plane = TRUE;
	}
	else
	{
		fields->t0 = (-1. - dist_to_plane) / normal_dot_velocity;
		fields->t1 = (1. - dist_to_plane) / normal_dot_velocity;
		if (fields->t0 > fields->t1)
			swapf(&fields->t0, &fields->t1);
		if (fields->t0 > 1 || fields->t1 < 0)
			return (FALSE);
		fields->t0 = clamp(0, 1, fields->t0);
		fields->t1 = clamp(0, 1, fields->t1);
	}
	return (TRUE);
}

t_bool				get_collision_point(t_physics_fields *fields,
	t_physics_data *packet, t_vec3 p[3])
{
	t_vec3	plane_intersect;
	t_vec3	tmp;

	fields->t = 1;
	if (!get_delays(fields, packet))
		return (FALSE);
	if (!fields->in_plane)
	{
		plane_intersect = ft_vec3_sub(packet->e_base_point,
			fields->plane.normal);
		tmp = ft_vec3_mul_s(packet->e_velocity, fields->t0);
		plane_intersect = ft_vec3_add(plane_intersect, tmp);
		if (point_in_triangle(plane_intersect, p[0], p[1], p[2]))
		{
			fields->found_collision = TRUE;
			fields->t = fields->t0;
			fields->colision_point = plane_intersect;
		}
	}
	return (TRUE);
}

void				finalize_collision(t_renderable *r,
	t_physics_fields *fields, t_physics_data *packet, t_vec3 p[3])
{
	float dist_to_coll;

	dist_to_coll = fields->t * ft_vec3_len(packet->e_velocity);
	if (!packet->found_colision || dist_to_coll < packet->distance)
	{
		packet->distance = dist_to_coll;
		packet->intersect_point = fields->colision_point;
		packet->found_colision = TRUE;
		packet->r = r;
		packet->a = p[0];
		packet->b = p[1];
		packet->c = p[2];
		packet->plane = fields->plane;
	}
	packet->grounded = (ft_vec3_dot(ft_vec3_norm(ft_vec3_sub(
		fields->colision_point, packet->e_velocity)),
		(t_vec3){ 0, 1, 0 }) <= -0.5);
}

t_physics_data		*check_triangle(t_renderable *r,
	t_physics_data *packet, t_vec3 p[3])
{
	t_physics_fields	fields;

	packet->r = NULL;
	fields = (t_physics_fields){ .plane = triangle_to_plane(p[0], p[1], p[2]) };
	if (!is_front_facing(fields.plane, packet->e_norm_velocity))
		return (packet);
	if (!get_collision_point(&fields, packet, p))
		return (packet);
	if (!fields.found_collision)
	{
		fields.velocity_sq_len = ft_vec3_dot(packet->e_velocity,
			packet->e_velocity);
		fields.a[0] = fields.velocity_sq_len;
		lowest_collision(&fields, packet, p[0]);
		lowest_collision(&fields, packet, p[1]);
		lowest_collision(&fields, packet, p[2]);
		edge_collision(&fields, packet, (t_vec3[2]){ p[1], p[0] });
		edge_collision(&fields, packet, (t_vec3[2]){ p[2], p[1] });
		edge_collision(&fields, packet, (t_vec3[2]){ p[0], p[2] });
	}
	if (fields.found_collision)
		finalize_collision(r, &fields, packet, p);
	return (packet);
}
