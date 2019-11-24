/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:00:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/24 18:36:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <limits.h>
#include "doom.h"
#include "maths/plane.h"
#include "entity.h"
#include "render.h"
#include "octree.h"
#include "sprite.h"

struct	s_entity_collision_check
{
	t_entity		*entity;
	t_renderable	*r;
};

void		collide_with_face(int face, void *p)
{
	struct s_entity_collision_check	*data;
	t_face							*f;

	data = p;
	f = &data->r->faces->values[face];
	if (!f->has_collision)
		return;
	if (f->normal_type == 1 || f->normal_type == 2)
	{
		check_triangle(data->r,
			&data->entity->packet, 
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[0] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[1] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[2] - 1]), data->entity->packet.e_radius)
		);
	}
	if (f->normal_type == 0 || f->normal_type == 2)	
	{
		check_triangle(data->r,
			&data->entity->packet, 
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[2] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[1] - 1]), data->entity->packet.e_radius),
			ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[0] - 1]), data->entity->packet.e_radius)
		);
	}
}

void		collide_with_octree(t_renderable *r, t_entity *entity, t_octree_node *octree, t_collide_aabb area)
{
	t_vec3 min = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	t_vec3 max = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
	min.x = fmin(min.x, area.min.x);
	min.y = fmin(min.y, area.min.y);
	min.z = fmin(min.z, area.min.z);
	min.x = fmin(min.x, area.max.x);
	min.y = fmin(min.y, area.max.y);
	min.z = fmin(min.z, area.max.z);

	max.x = fmax(max.x, area.min.x);
	max.y = fmax(max.y, area.min.y);
	max.z = fmax(max.z, area.min.z);
	max.x = fmax(max.x, area.max.x);
	max.y = fmax(max.y, area.max.y);
	max.z = fmax(max.z, area.max.z);

	area.min = min;
	area.max = max;
	aabb_intersect_octree(octree, &area, collide_with_face, &(struct s_entity_collision_check){ .entity = entity, .r = r });
}

void		check_collision(t_entity *entity, t_collide_aabb area)
{
	int				i;
	int				j;
	t_renderable	r;
	t_face			f;
	t_collide_aabb	new_area;
	
	i = -1;
	while (++i < entity->packet.doom->renderables->len)
	{
		r = entity->packet.doom->renderables->values[i];
		if (r.of.data.entity == entity || (r.of.type == RENDERABLE_ENTITY && (r.of.data.entity->type == ENTITY_GRENADA || r.of.data.entity->of.enemy.died)))
			continue;
		new_area = area;
		t_physics_data data = entity->packet;
		if (r.has_hitbox && r.hitbox.type == COLLIDE_ELLIPSOID)
		{
			t_collide_ellipsoid ellipsoid = r.hitbox.data.ellipsoid;
			t_renderable *sphere = &entity->packet.doom->sphere_primitive;
			sphere->position = ellipsoid.origin;
			sphere->scale = ellipsoid.radius;
			sphere->dirty = TRUE;
			transform_renderable(sphere);
			new_area.min = point_to_local(new_area.min, ellipsoid.origin, (t_vec3){0, 0, 0}, ellipsoid.radius);
			new_area.max = point_to_local(new_area.max, ellipsoid.origin, (t_vec3){0, 0, 0}, ellipsoid.radius);
		
			collide_with_octree(sphere, entity, sphere->octree, new_area);
			if (data.distance > entity->packet.distance)
				entity->packet.r = &entity->packet.doom->renderables->values[i];
		}
		else if (r.octree && r.faces->len > 100)
		{
			new_area.min = point_to_local(new_area.min, r.position, r.rotation, r.scale);
			new_area.max = point_to_local(new_area.max, r.position, r.rotation, r.scale);
			collide_with_octree(&r, entity, r.octree, new_area);
		}
		else
		{
			j = -1;
			while (++j < r.faces->len)
				collide_with_face(j, &(struct s_entity_collision_check){ .entity = entity, .r = &r });
		}
		if (entity->packet.r && entity->packet.r->of.type == RENDERABLE_ITEMSTACK)
		{
			if (entity_hit_itemstack(entity, entity->packet.r->of.data.itemstack))
			{
				printf("AMOUNT %d\n", entity->packet.r->of.data.itemstack->amount);
				if (entity->packet.r->of.data.itemstack->amount <= 0)
				{
					splice_renderables_array(entity->packet.doom->renderables, i, 1);
					i--;
				}
				entity->packet = data;
			}
		}
	}
}

void		check_grounded(t_entity *entity)
{
	// t_plane	plane;
	// float	f;

	// if (!entity->packet.found_colision)
	// 	return ;
	// f = ft_vec3_dot(entity->packet.plane.normal, (t_vec3){0, 1, 0});
	// if (f >= 0.8 && entity->velocity.y <= 0 && entity->packet.intersect_point.y <= entity->position.y)
	// 	entity->grounded = TRUE;
	// else
	// 	entity->grounded = FALSE;
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
	float very_close_dist = 0.0005 * unit_scale;

	if (entity->packet.depth > 5)
		return e_position;

	entity->packet.e_velocity = e_velocity;
	entity->packet.e_norm_velocity = ft_vec3_norm(e_velocity);
	entity->packet.e_base_point = e_position;
	entity->packet.found_colision = FALSE;
	entity->packet.distance = 1e20;

	float scale = fmax(1.5, ft_vec3_len(e_velocity)) * 1.5;
	t_vec3 r3_position = ft_vec3_mul(e_position, entity->packet.e_radius);
	t_vec3 query_radius = ft_vec3_mul_s(entity->packet.e_radius, scale);
	t_vec3 min = ft_vec3_sub(r3_position, query_radius);
	t_vec3 max = ft_vec3_add(r3_position, query_radius);
	check_collision(entity, (t_collide_aabb){ .min = min, .max = max });
	check_grounded(entity);

	t_vec3 dest_point = ft_vec3_add(e_position, e_velocity);
	if (entity->packet.found_colision == FALSE)
	{
		entity->grounded = FALSE;
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

	
	if (entity->type != ENTITY_GRENADA && entity->packet.intersect_point.y <= e_position.y && sliding_plane.normal.y > 0.95 && e_velocity.y < 0)
	{
			entity->grounded = TRUE;
			return new_base_point;
	}
	// entity->grounded = FALSE;
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
	entity->packet.r3_velocity = ft_vec3_mul_s(entity->velocity, entity->packet.dt);
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
	//entity->packet.r3_velocity = (t_vec3){0, 0, 0};
	entity->packet.r3_posision = ft_vec3_mul(final_pos, entity->packet.e_radius);
}

void		entity_update(t_doom *doom, t_entity *entity, double dt)
{
	ALint status;

	if (entity->type == ENTITY_ENEMY)
	{
		entity_update_enemy(doom, entity, dt);
		if ((entity->velocity.x || entity->velocity.z) && entity->grounded)
		{
				// && entity->velocity.y == 0)
			alGetSourcei(entity->sources[2], AL_SOURCE_STATE, &status);
			if (status != AL_PLAYING)
				entity_sound(entity, 9, 2, 1);
		}
			// play_music(&doom->audio, entity->position, 9, TRUE);
	}
	if (entity->type == ENTITY_PLAYER
			&& (entity->velocity.x || entity->velocity.z)
			&& entity->grounded
			&& doom->audio.source_status[CHAR_FOOTSTEP] == 0)
		player_sound(&doom->audio, CHAR_FOOTSTEP, 9, 1);
	if (entity->type == ENTITY_GRENADA)
	{
		entity->velocity.y -= 0.9;
		entity->velocity.y = fmax(-4, entity->velocity.y);
	}
	else
	{	
		if (entity->grounded && entity->jump && entity->velocity.y < 0)
			entity->velocity.y = fmax(0, entity->velocity.y);
		if (entity->jump && entity->velocity.y <= 10 && !entity->packet.found_colision)
			entity->velocity.y += 1.5;
		if (entity->jump && entity->velocity.y >= 10)
			entity->jump = FALSE;
		if (!entity->jump)
		{
			entity->velocity.y -= !entity->grounded ? 8 : 40;
			entity->velocity.y = fmax(-40, entity->velocity.y);
		}
	}
	entity->packet.r3_posision = entity->position;
	entity->packet.r3_velocity = entity->velocity;
	entity->packet.e_radius = entity->radius;
	entity->packet.grounded = FALSE;
	entity->packet.dt = dt;
	entity->packet.doom = doom;
	collide_and_slide(entity);
	if (entity->grounded)
		entity->jump = FALSE;

	entity->position = entity->packet.r3_posision;
	if (entity->type == ENTITY_GRENADA)
		entity->velocity = ft_vec3_mul_s(entity->velocity, 0.999);
	else if (entity->type == ENTITY_ENEMY)
		entity->velocity = (t_vec3){ 0, 0, 0 };
	else
	{
		entity->velocity.x *= !entity->grounded && !entity->packet.found_colision ? 0.99 : 0.5;
		entity->velocity.z *= !entity->grounded && !entity->packet.found_colision ? 0.99 : 0.5;
		if (entity->velocity.x < 1 && entity->velocity.x > -1)
			entity->velocity.x = 0;
		if (entity->velocity.z < 1 && entity->velocity.z > -1)
			entity->velocity.z = 0;
	}
	entity->radius = entity->packet.e_radius;
//	entity->grounded = entity->packet.grounded;
}
