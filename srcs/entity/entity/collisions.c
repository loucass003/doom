/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:32:09 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 18:16:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "render.h"
#include "entity.h"
#include "doom.h"
#include "octree.h"

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

void			check_triangle_normal_type(t_entity_collision_check *data,
	t_face *f)
{
	if (f->normal_type == 1 || f->normal_type == 2)
	{
		check_triangle(data->r, &data->entity->packet,
		(t_vec3[3]) {
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[0] - 1]),
		data->entity->packet.e_radius),
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[1] - 1]),
		data->entity->packet.e_radius),
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[2] - 1]),
		data->entity->packet.e_radius) });
	}
	if (f->normal_type == 0 || f->normal_type == 2)
	{
		check_triangle(data->r, &data->entity->packet,
		(t_vec3[3]) {
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[2] - 1]),
		data->entity->packet.e_radius),
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[1] - 1]),
		data->entity->packet.e_radius),
		ft_vec3_div(vec4_to_3(data->r->pp_vertices[f->vertices_index[0] - 1]),
		data->entity->packet.e_radius)});
	}
}

void			collide_with_face(int face, void *p)
{
	t_entity_collision_check		*data;
	t_face							*f;

	data = p;
	f = &data->r->faces->values[face];
	if (!f->has_collision)
		return ;
	check_triangle_normal_type(data, f);
}

void			collide_with_octree(t_renderable *r, t_entity *entity,
	t_octree_node *octree, t_collide_aabb area)
{
	t_vec3 min;
	t_vec3 max;

	min = (t_vec3){ INT_MAX, INT_MAX, INT_MAX };
	max = (t_vec3){ INT_MIN, INT_MIN, INT_MIN };
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
	aabb_intersect_octree(octree, &area, collide_with_face,
		&(t_entity_collision_check){ .entity = entity, .r = r });
}

void			check_col_collide_ellipsoid(t_renderable r, t_entity *entity,
	t_collide_aabb new_area, int i)
{
	t_collide_ellipsoid	ellipsoid;
	t_renderable		*sphere;
	t_physics_data		data;

	data = entity->packet;
	ellipsoid = r.hitbox.data.ellipsoid;
	sphere = &entity->packet.doom->sphere_primitive;
	sphere->position = ellipsoid.origin;
	sphere->scale = ellipsoid.radius;
	sphere->dirty = TRUE;
	transform_renderable(sphere);
	new_area.min = point_to_local(new_area.min, ellipsoid.origin,
		(t_vec3){0, 0, 0}, ellipsoid.radius);
	new_area.max = point_to_local(new_area.max, ellipsoid.origin,
		(t_vec3){0, 0, 0}, ellipsoid.radius);
	collide_with_octree(sphere, entity, sphere->octree, new_area);
	if (data.distance > entity->packet.distance)
		entity->packet.r = &entity->packet.doom->renderables->values[i];
}

t_bool			check_collision_rocket(t_entity *entity, t_physics_data data)
{
	entity_sound(entity, 8, 0, 1);
	damage_explo(entity, entity->packet.doom, entity->of.rocket.damage);
	splice_renderables_array(entity->packet.doom->renderables,
		renderables_indexof(entity->packet.doom->renderables,
			entity->r), 1);
	entity->packet = data;
	return (FALSE);
}

int				check_col_itemstack(t_entity *entity, int *i,
	t_physics_data data)
{
	if (entity_hit_itemstack(entity,
		entity->packet.r->of.data.itemstack))
	{
		if (entity->packet.r->of.data.itemstack->amount <= 0)
		{
			splice_renderables_array(entity->packet.doom->renderables, i, 1);
			*(i--);
		}
		entity->packet = data;
	}
}

t_collide_aabb	check_col_octree(t_collide_aabb new_area, t_renderable r,
	t_entity *entity, int i)
{
	t_physics_data	data;

	data = entity->packet;
	new_area.min = point_to_local(new_area.min, r.position, r.rotation,
		r.scale);
	new_area.max = point_to_local(new_area.max, r.position, r.rotation,
		r.scale);
	collide_with_octree(&r, entity, r.octree, new_area);
	if (data.distance > entity->packet.distance)
		entity->packet.r = &entity->packet.doom->renderables->values[i];
	return (new_area);
}

void			check_col_else(t_renderable r, t_entity *entity)
{
	int	j;

	j = -1;
	while (++j < r.faces->len)
		collide_with_face(j, &(t_entity_collision_check){
			.entity = entity, .r = &r });
}

void			check_col(t_renderable r, t_entity *entity,
	t_collide_aabb new_area, int i)
{
	if (r.has_hitbox && r.hitbox.type == COLLIDE_ELLIPSOID)
		check_col_collide_ellipsoid(r, entity, new_area, i);
	else if (r.octree && r.faces->len > 100)
		new_area = check_col_octree(new_area, r, entity, i);
	else
		check_col_else(r, entity);
}

t_bool				entity_hit_something(t_entity *entity, int *i)
{
	if (entity->packet.r
		&& entity->packet.r->of.type == RENDERABLE_ITEMSTACK)
		check_col_itemstack(entity, i, entity->packet);
	if (entity->type == ENTITY_ROCKET && entity->packet.found_colision)
		return (check_collision_rocket(entity, entity->packet));
	if (entity->packet.r && entity->packet.r->of.type == RENDERABLE_TRANSPO
		&& renderable_transpo(entity) == FALSE)
		return (FALSE);
	return (TRUE);
}

t_bool				check_collision(t_entity *entity, t_collide_aabb area)
{
	int				i;
	t_renderable	r;
	t_collide_aabb	new_area;

	i = -1;
	while (++i < entity->packet.doom->renderables->len)
	{
		r = entity->packet.doom->renderables->values[i];
		entity->packet.r = NULL;
		if (r.of.data.entity == entity || (r.of.type == RENDERABLE_ENTITY
			&& (r.of.data.entity->type == ENTITY_GRENADA
				|| (r.of.data.entity->dead))))
			continue;
		if (entity->type == ENTITY_ROCKET && r.of.type == RENDERABLE_ENTITY
			&& r.of.data.entity->type == ENTITY_BOSS)
			continue;
		if (entity->type == ENTITY_GRENADA && r.of.type == RENDERABLE_ENTITY
			&& r.of.data.entity->type == ENTITY_PLAYER)
			continue;
		new_area = area;
		check_col(r, entity, new_area, i);
		entity_hit_something(entity, &i);
	}
	return (TRUE);
}

t_vec3		collide_with_world(t_entity *entity, t_vec3 e_position,
	t_vec3 e_velocity, t_bool *stop)
{
	float	unit_scale;
	float	very_close_dist;

	unit_scale = 1;
	very_close_dist = 0.0005 * unit_scale;
	if (entity->packet.depth > 5)
		return (e_position);
	entity->packet.e_velocity = e_velocity;
	entity->packet.e_norm_velocity = ft_vec3_norm(e_velocity);
	entity->packet.e_base_point = e_position;
	entity->packet.found_colision = FALSE;
	entity->packet.distance = 1e20;

	float	scale;
	t_vec3	r3_position;
	t_vec3	query_radius;
	t_vec3	min;
	t_vec3	max;
	t_vec3	dest_point;

	scale = fmax(1.5, ft_vec3_len(e_velocity)) * 1.5;
	r3_position = ft_vec3_mul(e_position, entity->packet.e_radius);
	query_radius = ft_vec3_mul_s(entity->packet.e_radius, scale);
	min = ft_vec3_sub(r3_position, query_radius);
	max = ft_vec3_add(r3_position, query_radius);
	if (*stop || !check_collision(entity, (t_collide_aabb){
		.min = min, .max = max }))
	{
		*stop = TRUE;
		return (ft_vec3_add(e_position, e_velocity));
	}

	dest_point = ft_vec3_add(e_position, e_velocity);
	if (entity->packet.found_colision == FALSE)
	{
		entity->grounded = FALSE;
		return (dest_point);
	}

	t_vec3	new_base_point;

	new_base_point = e_position;
	if (entity->packet.distance >= very_close_dist)
	{
		t_vec3	v;
		v = ft_vec3_trim(e_velocity, entity->packet.distance - very_close_dist);
		new_base_point = ft_vec3_add(entity->packet.e_base_point, v);
		v = ft_vec3_norm(v);
		entity->packet.intersect_point = ft_vec3_sub(entity->packet.
			intersect_point, ft_vec3_mul_s(v, very_close_dist));
	}

	t_vec3	slide_plane_origin;
	t_vec3	slide_plane_normal;

	slide_plane_origin = entity->packet.intersect_point;
	slide_plane_normal = ft_vec3_norm(ft_vec3_sub(new_base_point,
		entity->packet.intersect_point));

	t_plane	sliding_plane;
	float	slide_factor;
	sliding_plane = plane_new(slide_plane_origin, slide_plane_normal);
	slide_factor = distance_to_plane(sliding_plane, dest_point);

	if (entity->type != ENTITY_GRENADA
		&& entity->packet.intersect_point.y <= e_position.y
		&& sliding_plane.normal.y > 0.95 && e_velocity.y < 0)
	{
		entity->grounded = TRUE;
		return (new_base_point);
	}
	t_vec3	new_dest_point;
	t_vec3	new_velocity;

	new_dest_point = ft_vec3_sub(dest_point, ft_vec3_mul_s(slide_plane_normal,
		slide_factor));
	new_velocity = ft_vec3_sub(new_dest_point, entity->packet.intersect_point);
	if (ft_vec3_len(new_velocity) < very_close_dist)
		return (new_base_point);
	entity->packet.depth++;
	return collide_with_world(entity, new_base_point, new_velocity, stop);
}

t_bool		collide_and_slide(t_entity *e)
{
	t_vec3	gravity;
	t_vec3	e_position;
	t_vec3	e_velocity;
	t_vec3	final_pos;
	t_bool	stop;

	e->packet.r3_posision = e->position;
	e->packet.r3_velocity = ft_vec3_mul_s(e->velocity, e->packet.dt);
	e->packet.e_radius = e->radius;
	gravity = (t_vec3){0, fmin(0, e->packet.r3_velocity.y), 0};
	e_position = ft_vec3_div(e->packet.r3_posision, e->packet.e_radius);
	e_velocity = ft_vec3_div(e->packet.r3_velocity, e->packet.e_radius);
	e_velocity.y = fmax(0.0, e_velocity.y);
	e->packet.depth = 0;
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