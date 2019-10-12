/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:00:26 by llelievr          #+#    #+#             */
/*   Updated: 2019/10/12 03:21:38 by llelievr         ###   ########.fr       */
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
	// if (f->hidden)
	// 	return;
	check_triangle(data->r,
		&data->entity->packet, 
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[0] - 1]), data->entity->packet.e_radius),
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[1] - 1]), data->entity->packet.e_radius),
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[2] - 1]), data->entity->packet.e_radius)
	);
}

// t_collision		sphere_colliding_sphere(t_vec3 translation1, t_vec3 translation2, t_vec3 velocitySum, float radiiSum)
// {
// 	float	distance_between = ft_vec3_len(ft_vec3_sub(translation1, translation2)) - radiiSum;
// 	float	velocityLength = ft_vec3_len(velocitySum);

// 	if (distance_between > 0 && velocityLength < distance_between)
// 		return ((t_collision){ .collide = FALSE, .dist = -1 });
// 	t_vec3	norm_velocity = ft_vec3_norm(velocitySum);
// 	t_vec3	direction = ft_vec3_sub(translation1, translation2);
// 	float	angle_between = ft_vec3_dot(norm_velocity, direction);

// 	if (angle_between < 0)
// 	{
// 		if (distance_between < 0)
// 		{
// 			if (velocityLength > -distance_between)
// 				return ((t_collision){ .collide = FALSE, .dist = -1 });
// 		}
// 		else
// 			return ((t_collision){ .collide = FALSE, .dist = -1 });
// 	}

// 	float	direction_len = ft_vec3_len(direction);
// 	float	hypotenuse = (direction_len * direction_len) - (angle_between * angle_between);
// 	float 	radiiSumSquared = radiiSum * radiiSum;
// 	if( hypotenuse >= radiiSumSquared )
// 		return ((t_collision){ .collide = FALSE, .dist = -1 });
// 	float distance = radiiSumSquared - hypotenuse;
// 	float collisionDistance = angle_between - sqrtf( distance );
// 	if (velocityLength < collisionDistance)
// 		return ((t_collision){ .collide = FALSE, .dist = -1 });
// 	return ((t_collision){ .collide = TRUE, .dist = collisionDistance });
// }

// void		collide_with_entity(t_entity *entity, t_collide_ellipsoid ellisioid, t_vec3 ellipsoid_velocity)
// {
// 	t_vec3	translation = ft_vec3_div(ellisioid.origin, entity->radius);
// 	t_vec3	velocity = ft_vec3_div(ellipsoid_velocity, entity->radius);

// 	t_vec3	vector_collider_object = ft_vec3_norm(ft_vec3_sub(translation, entity->position));
// 	t_vec3	vector_object_collider = ft_vec3_norm(ft_vec3_sub(entity->position, translation));

// 	float	collider_radius = ft_vec3_len(vector_collider_object);
// 	t_vec3	vector_object_radius = ft_vec3_mul(vector_object_collider, ft_vec3_div(ellisioid.radius, entity->radius));
// 	float	object_radius = ft_vec3_len(vector_object_radius);

// 	t_collision hit = sphere_colliding_sphere(entity->position, translation, ft_vec3_sub(velocity, entity->velocity), collider_radius + object_radius);
// 	if (hit.collide)
// 	{
// 		if (entity->packet.found_colision == FALSE || hit.dist < entity->packet.distance)
// 		{
// 			entity->packet.distance = hit.dist;
// 			entity->packet.intersect_point = ft_vec3_mul_s(entity->packet.e_norm_velocity, hit.dist);
// 			entity->packet.found_colision = TRUE;
// 		}
// 	}
// }

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

#include <sys/time.h>
long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
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
		if (r.of.type == RENDERABLE_ENTITY && r.of.data.entity == entity)
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

// void		check_grounded(t_entity *entity)
// {
// 	t_plane	plane;
// 	float	f;

// 	if (!entity->packet.found_colision)
// 		return ;
// 	plane = triangle_to_plane(
// 		ft_vec3_mul(entity->packet.a, entity->radius),
// 		ft_vec3_mul(entity->packet.b, entity->radius),
// 		ft_vec3_mul(entity->packet.c, entity->radius)
// 	);
// 	f = ft_vec3_dot(plane.normal, (t_vec3){0, 1, 0});
// 	if (f >= 0.8)
// 		entity->grounded = TRUE;
// }


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
	entity->packet.r3_velocity = (t_vec3){0, 0, 0};
	entity->packet.r3_posision = ft_vec3_mul(final_pos, entity->packet.e_radius);
}

void		entity_update(t_doom *doom, t_entity *entity, double dt)
{
	if (entity->type == ENTITY_ENEMY)
		entity_update_enemy(doom, entity, dt);
	entity->velocity.y -= 40;
	entity->packet.r3_posision = entity->position;
	entity->packet.r3_velocity = entity->velocity;
	entity->packet.e_radius = entity->radius;
	entity->packet.grounded = entity->grounded;
	entity->packet.dt = dt;
	entity->packet.doom = doom;
	collide_and_slide(entity);
	entity->position = entity->packet.r3_posision;
	entity->velocity = entity->packet.r3_velocity;
	entity->radius = entity->packet.e_radius;
	entity->grounded = entity->packet.grounded;
}
