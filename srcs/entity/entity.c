/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:00:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/03 14:52:39 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "doom.h"
#include "maths/plane.h"
#include "entity.h"
#include "render.h"

void		check_collision(t_entity *entity)
{
	int				i;
	int				j;
	t_renderable	r;
	t_face			f;

	i = -1;
	while (++i < entity->packet.doom->renderables->len)
	{
		r = entity->packet.doom->renderables->values[i];
		if (r.entity == entity)
			continue;
		j = -1;
		while (++j < r.faces->len)
		{
			f = r.faces->values[j];
			check_triangle(&entity->packet, 
				ft_vec3_div(vec4_to_3(r.pp_vertices[f.vertices_index[0] - 1]), entity->packet.e_radius),
				ft_vec3_div(vec4_to_3(r.pp_vertices[f.vertices_index[1] - 1]), entity->packet.e_radius),
				ft_vec3_div(vec4_to_3(r.pp_vertices[f.vertices_index[2] - 1]), entity->packet.e_radius)
			);
		}
	}
}

void		check_grounded(t_entity *entity)
{
	t_plane	plane;
	float	f;

	if (!entity->packet.found_colision)
		return ;
	plane = triangle_to_plane(
		ft_vec3_mul(entity->packet.a, entity->radius),
		ft_vec3_mul(entity->packet.b, entity->radius),
		ft_vec3_mul(entity->packet.c, entity->radius)
	);
	f = ft_vec3_dot(plane.normal, (t_vec3){0, 1, 0});
	if (f >= 0.8)
		entity->grounded = TRUE;
}


t_vec3		ft_vec3_trim(t_vec3 v, float max_len)
{
	float len = ft_vec3_len(v);

	if (len > max_len)
	{
		len = 1.0 / len;
		v = ft_vec3_mul_s(v, len);
		v = ft_vec3_mul_s(v, max_len);
	}
	return v;
}

t_vec3		collide_with_world(t_entity *entity, t_vec3 e_position, t_vec3 e_velocity)
{
	float unit_scale = 1;
	float very_close_dist = 0.005 * unit_scale;

	if (entity->packet.depth > 5)
		return e_position;

	entity->packet.e_velocity = e_velocity;
	entity->packet.e_norm_velocity = ft_vec3_norm(e_velocity);
	entity->packet.e_base_point = e_position;
	entity->packet.found_colision = FALSE;
	entity->packet.distance = 1e20;

	// float scale = fmax(1.5, ft_vec3_len(e_velocity)) * 1.1;
	// t_vec3 r3_position = ft_vec3_mul(e_position, entity->packet.e_radius);
	// t_vec3 query_radius = ft_vec3_mul_s(entity->packet.e_radius, scale);
	// t_vec3 min = ft_vec3_sub(r3_position, query_radius);
	// t_vec3 max = ft_vec3_add(r3_position, query_radius);
	check_collision(entity);

	t_vec3 dest_point = ft_vec3_add(e_position, e_velocity);
	if (entity->packet.found_colision == FALSE)
	{
		return dest_point;
	}

	t_vec3 new_base_point = e_position;

	if (entity->packet.distance >= very_close_dist)
	{
		t_vec3 v = ft_vec3_trim(e_velocity, entity->packet.distance - very_close_dist);
		new_base_point = ft_vec3_add(entity->packet.e_base_point, v);
		v = ft_vec3_norm(v);
		entity->packet.intersect_point = ft_vec3_sub(entity->packet.intersect_point, ft_vec3_mul_s(v, very_close_dist));
	}

	t_vec3 slide_plane_origin = entity->packet.intersect_point;
	t_vec3 slide_plane_normal = ft_vec3_norm(ft_vec3_sub(new_base_point, entity->packet.intersect_point));

	t_plane sliding_plane = plane_new(slide_plane_origin, slide_plane_normal);
	float slide_factor = distance_to_plane(sliding_plane, dest_point);

	if (entity->packet.intersect_point.y <= e_position.y && sliding_plane.normal.y > 0.8 && e_velocity.y < 0)
		return new_base_point;
	t_vec3 new_dest_point = ft_vec3_sub(dest_point, ft_vec3_mul_s(slide_plane_normal, slide_factor));
	t_vec3 new_velocity = ft_vec3_sub(new_dest_point, entity->packet.intersect_point);
	if (ft_vec3_len(new_velocity) < very_close_dist)
		return new_base_point;
	entity->packet.depth++;
	return collide_with_world(entity, new_base_point, new_velocity);
}

void		collide_and_slide(t_entity *entity)
{	
	entity->packet.r3_posision = entity->position;
	entity->packet.r3_velocity = entity->velocity;
	entity->packet.e_radius = entity->radius;

	t_vec3	gravity = (t_vec3){0, fmin(0, entity->packet.r3_velocity.y), 0};
	t_vec3	e_position = ft_vec3_div(entity->packet.r3_posision, entity->packet.e_radius);
	t_vec3	e_velocity = ft_vec3_div(entity->packet.r3_velocity, entity->packet.e_radius);
	t_vec3	final_pos;

	e_velocity.y = fmax(0.0, e_velocity.y);

	entity->packet.depth = 0;
	final_pos = collide_with_world(entity, e_position, e_velocity);

	entity->packet.r3_posision = ft_vec3_mul(final_pos, entity->packet.e_position);
	entity->packet.e_velocity = gravity;

	e_velocity = ft_vec3_div(gravity, entity->packet.e_radius);
	entity->packet.depth = 0;
	final_pos = collide_with_world(entity, final_pos, e_velocity);
	entity->packet.r3_velocity = (t_vec3){0, 0, 0};
	entity->packet.r3_posision = ft_vec3_mul(final_pos, entity->packet.e_radius);
}

void		entity_update(t_doom *doom, t_entity *entity, double dt)
{
	if (entity->type == ENTITY_ENEMY)
		entity_update_enemy(doom, entity, dt);
	entity->velocity.y -= 1;
	entity->packet.r3_posision = entity->position;
	entity->packet.r3_velocity = entity->velocity;
	entity->packet.e_radius = entity->radius;
	entity->packet.grounded = entity->grounded;
	entity->packet.doom = doom;
	collide_and_slide(entity);
	entity->position = entity->packet.r3_posision;
	entity->velocity = entity->packet.r3_velocity;
	entity->radius = entity->packet.e_radius;
	entity->grounded = entity->packet.grounded;

	
}
