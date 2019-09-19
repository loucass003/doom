/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:00:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/19 05:23:56 by llelievr         ###   ########.fr       */
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

void		collide_with_world(t_entity *entity, t_vec3 e_position, t_vec3 e_velocity)
{
	t_plane	first_plane;
	t_vec3	dest = ft_vec3_add(e_position, e_velocity);
	t_vec3	tmp;
	int		i;

	i = -1;
	while (++i < 3)
	{
		tmp = ft_vec3_norm(e_velocity);
		entity->packet.e_norm_velocity = tmp;
		entity->packet.e_velocity = e_velocity;
		entity->packet.e_base_point = e_position;
		entity->packet.e_radius = entity->radius;
		entity->packet.found_colision = FALSE;
		entity->packet.distance = INT_MAX;
		entity->packet.t = 0;

		check_collision(entity);
		check_grounded(entity);

		if (!entity->packet.found_colision)
		{
			e_position = dest;
			return ;
		}

		printf("STOP %f %f %f\n", dest.x, dest.y, dest.z);

		t_vec3 touch_point;

		tmp = ft_vec3_mul_s(e_velocity, entity->packet.t);
		touch_point = ft_vec3_add(e_position, tmp);

		float dist = ft_vec3_len(e_velocity) * entity->packet.t;
		float short_dist = fmax(dist - 1, 0);

		tmp = ft_vec3_norm(e_velocity);//TODO: WHUT
		tmp = ft_vec3_mul_s(tmp, short_dist);
		e_position = ft_vec3_add(e_position, tmp);
		
		t_vec3 slide_plane_origin = entity->packet.intersect_point;
		t_vec3 slide_plane_normal = ft_vec3_norm(ft_vec3_sub(touch_point, entity->packet.intersect_point));
		
		if (i == 0)
		{
			float long_radius = 1.0 + 0.001;
			first_plane = plane_new(slide_plane_origin, slide_plane_normal);

			float dist_to_plane = distance_to_plane(first_plane, dest) - long_radius;

			tmp = ft_vec3_mul_s(first_plane.normal, dist_to_plane);
			dest = ft_vec3_sub(dest, tmp);
			e_velocity = ft_vec3_sub(dest, e_position);
		}
		else if (i == 1)
		{
			t_plane second_plane = plane_new(slide_plane_origin, slide_plane_normal);

			t_vec3 crease = ft_vec3_cross(first_plane.normal, second_plane.normal);

			tmp = ft_vec3_sub(dest, e_position);
			float dis = ft_vec3_dot(tmp, crease);
			crease = ft_vec3_norm(crease);
			
			e_velocity = ft_vec3_mul_s(crease, dis);
			dest = ft_vec3_add(e_position, e_velocity);
		}
	}	
	e_position = dest;
}

void		collide_and_slide(t_entity *entity)
{	
	entity->packet.r3_posision = entity->position;
	entity->packet.r3_velocity = entity->velocity;
	entity->packet.e_radius = entity->radius;

	t_vec3	gravity = (t_vec3){ 0, entity->packet.r3_velocity.y, 0 };
	entity->packet.r3_velocity.y = 0;

	t_vec3	e_position = ft_vec3_div(entity->packet.r3_posision, entity->packet.e_radius);
	t_vec3	e_velocity = ft_vec3_div(entity->packet.r3_velocity, entity->packet.e_radius);
	collide_with_world(entity, e_position, e_velocity);

	entity->packet.r3_velocity = gravity;
	e_velocity = ft_vec3_div(gravity, entity->packet.e_radius);
	collide_with_world(entity, e_position, e_velocity);
	
	entity->position = ft_vec3_mul(e_position, entity->packet.e_radius);
}

void		entity_update(t_entity *entity, double dt)
{
	t_vec3	xz;
	int		i;

	xz = entity->velocity;
	xz.y = 0;

	if (entity->grounded && fabs(ft_vec3_len(xz)) < 0.1 && entity->velocity.y < 0)
		entity->velocity.y = 0;
	else
		entity->grounded = FALSE;
	dt = dt / 5.;
	
	entity->velocity = ft_vec3_mul_s(entity->velocity, dt);
	
	i = -1;
	while (++i < 5)
		collide_and_slide(entity);
	printf("%f %f %f - %f %f %f\n", 
		entity->position.x,
		entity->position.y,
		entity->position.z,
		entity->velocity.x,
		entity->velocity.y,
		entity->velocity.z);
	entity->velocity = ft_vec3_sub(entity->position, entity->packet.r3_posision);
	entity->velocity = ft_vec3_mul_s(entity->velocity, 1.0 / dt);
	printf(" ----$ %f %f %f - %f %f %f\n", 
			entity->position.x,
			entity->position.y,
			entity->position.z,
			entity->velocity.x,
			entity->velocity.y,
			entity->velocity.z);
}
