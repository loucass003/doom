/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:32:09 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/17 19:54:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "render.h"
#include "entity.h"
#include "doom.h"
#include "octree.h"

void		collide_reset_packet(t_entity *entity, t_vec3 e_position,
	t_vec3 e_velocity)
{
	entity->packet.e_velocity = e_velocity;
	entity->packet.e_norm_velocity = ft_vec3_norm(e_velocity);
	entity->packet.e_base_point = e_position;
	entity->packet.found_colision = FALSE;
	entity->packet.distance = 1e20;
}

t_vec3		close_dist_point(t_entity *entity, t_vec3 e_velocity)
{
	t_vec3	v;
	t_vec3	new_base_point;

	v = ft_vec3_trim(e_velocity, entity->packet.distance - VERRY_CLOSE_DIST);
	new_base_point = ft_vec3_add(entity->packet.e_base_point, v);
	v = ft_vec3_norm(v);
	entity->packet.intersect_point = ft_vec3_sub(entity->packet.
		intersect_point, ft_vec3_mul_s(v, VERRY_CLOSE_DIST));
	return (new_base_point);
}

t_vec3		collide_with_world(t_entity *e, t_vec3 ep,
	t_vec3 ev, t_bool *s)
{
	t_vec3	dest;
	t_vec3	new;
	t_plane	plane;
	t_vec3	vel;

	if (e->packet.depth > 5 && !!(++e->packet.depth))
		return (ep);
	collide_reset_packet(e, ep, ev);
	if ((*s || !check_collision(e, get_max_test_range(e, ep, ev))) && (*s = 1))
		return (ft_vec3_add(ep, ev));
	dest = ft_vec3_add(ep, ev);
	if (e->packet.found_colision == FALSE && !(e->grounded = FALSE))
		return (dest);
	new = ep;
	if (e->packet.distance >= VERRY_CLOSE_DIST)
		new = close_dist_point(e, ev);
	plane = plane_new(e->packet.intersect_point, ft_vec3_norm(
		ft_vec3_sub(new, e->packet.intersect_point)));
	if (entity_cant_slide(e, ep, ev, plane) && (e->grounded = TRUE))
		return (new);
	vel = ft_vec3_sub(ft_vec3_sub(dest, ft_vec3_mul_s(plane.normal,
		distance_to_plane(plane, dest))), e->packet.intersect_point);
	if (ft_vec3_len(vel) < VERRY_CLOSE_DIST)
		return (new);
	return (collide_with_world(e, new, vel, s));
}

void		init_collide_and_slide(t_entity *e)
{
	e->packet.r3_posision = e->position;
	e->packet.r3_velocity = ft_vec3_mul_s(e->velocity, e->packet.dt);
	e->packet.e_radius = e->radius;
	e->packet.depth = 0;
}

t_bool		collide_and_slide(t_entity *e)
{
	t_vec3	gravity;
	t_vec3	e_position;
	t_vec3	e_velocity;
	t_vec3	final_pos;
	t_bool	stop;

	init_collide_and_slide(e);
	gravity = (t_vec3){0, fmin(0, e->packet.r3_velocity.y), 0};
	e_position = ft_vec3_div(e->packet.r3_posision, e->packet.e_radius);
	e_velocity = ft_vec3_div(e->packet.r3_velocity, e->packet.e_radius);
	e_velocity.y = fmax(0.0, e_velocity.y);
	stop = FALSE;
	final_pos = collide_with_world(e, e_position, e_velocity, &stop);
	if (stop)
		return (FALSE);
	e->packet.r3_posision = ft_vec3_mul(final_pos, e->packet.e_position);
	e->packet.e_velocity = gravity;
	e_velocity = ft_vec3_div(gravity, e->packet.e_radius);
	e->packet.depth = 0;
	stop = FALSE;
	final_pos = collide_with_world(e, final_pos, e_velocity, &stop);
	if (stop)
		return (FALSE);
	e->packet.r3_posision = ft_vec3_mul(final_pos, e->packet.e_radius);
	return (TRUE);
}
