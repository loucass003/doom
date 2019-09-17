/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 23:50:58 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/17 16:45:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <limits.h>
#include "maths/triangle.h"
#include "doom.h"
#include "collision.h"

typedef struct	s_plane 
{
	t_vec3	origin;
	t_vec3	normal;
	t_vec3	point;
	float	f;
}				t_plane;

t_plane	triangle_to_plane(t_vec3 a, t_vec3 b, t_vec3 c)
{
	const t_vec3	normal = get_triangle_normal(a, b, c);

	return ((t_plane) {
		.origin = a,
		.normal = normal,
		.point = normal,
		.f = -ft_vec3_dot(normal, a)
	});
}

t_plane	plane_new(t_vec3 origin, t_vec3 normal)
{
	return ((t_plane){
		.origin = origin,
		.normal = normal,
		.point = normal,
		.f = -ft_vec3_dot(normal, origin)
	});
}

float	distance_to_plane( t_plane plane, t_vec3 point)
{
	return ft_vec3_dot(point, plane.normal) + plane.f;
}

t_bool	is_front_facing(t_plane plane, t_vec3 direction)//TODO: probably need fix 
{
	const float f = ft_vec3_dot(plane.normal, direction);

	return (f <= 0);
}

t_bool	point_in_triangle(t_vec3 point, t_vec3 p1, t_vec3 p2, t_vec3 p3)
{
	const t_vec3	u = ft_vec3_sub(p2, p1);//TODO: probably inversed too
	const t_vec3	v = ft_vec3_sub(p3, p1);
	const t_vec3	w = ft_vec3_sub(point, p1);
	const t_vec3	vw = ft_vec3_cross(v, w);
	const t_vec3	vu = ft_vec3_cross(v, u);

	if (ft_vec3_dot(vw, vu) < 0)
		return (FALSE);
	
	t_vec3 uw = ft_vec3_cross(u, w);
	t_vec3 uv = ft_vec3_cross(u, v);
	if (ft_vec3_dot(uw, uv) < 0)
		return (FALSE);
	
	float d = ft_vec3_len(uv);
	float r = ft_vec3_len(vw) / d;
	float t = ft_vec3_len(uw) / d;

	return ((r + t) <= 1);
}

t_bool	lowest_root(t_vec3 v, float max, float *root)
{
	const float det = v.y * v.y - 4.0 * v.x * v.z;

	if (det < 0)
		return (FALSE);
	float sqrt_d = sqrtf(det);
	float r1 = (-v.y - sqrt_d) / (2.0 * v.x);
	float r2 = (-v.y + sqrt_d) / (2.0 * v.x);
	if (r1 > r2) 
	{
		float tmp = r2;
		r2 = r1;
		r1 = tmp;
	}
	if (r1 > 0 && r1 < max)
	{
		*root = r1;
		return (TRUE);
	}
	if (r2 > 0 && r2 < max)
	{
		*root = r2;
		return (TRUE);
	}
	return (FALSE);
}

float				clamp(float min, float max, float v)
{
	return (fmin(max, fmax(min, v))); //TODO: i alwas inverse those
}

t_physics_data		*check_triangle(t_physics_data *packet, t_vec3 p1, t_vec3 p2, t_vec3 p3)
{
	const	t_plane plane = triangle_to_plane(p1, p2, p3);

	if (!is_front_facing(plane, packet->e_norm_velocity))
		return packet;
	
	float	t0 = 0;
	float	t1 = 0;
	t_bool	in_plane = FALSE;

	float dist_to_plane = distance_to_plane(plane, packet->e_base_point);
	float normal_dot_velocity = ft_vec3_dot(plane.normal, packet->e_velocity);

	if (normal_dot_velocity == 0)
	{
		if (fabsf(dist_to_plane) >= 1)
			return packet;
		in_plane = TRUE;
		t0 = 0;
		t1 = 1;
	}
	else
	{
		t0 = (-1. - dist_to_plane) / normal_dot_velocity;
		t1 = (1. - dist_to_plane) / normal_dot_velocity;
		if (t0 > t1)
		{
			float tmp = t1;
			t1 = t0;
			t0 = tmp;
		}

		if (t0 > 1 || t1 < 0)
			return (packet);
		t0 = clamp(0, 1, t0);
		t1 = clamp(0, 1, t1);
	}

	t_vec3	colision_point = (t_vec3){0, 0, 0};
	t_bool	found_collision = FALSE;
	float	t = 1;

	if (!in_plane)
	{
		t_vec3 plane_intersect = ft_vec3_sub(packet->e_base_point, plane.normal);
		t_vec3 tmp = ft_vec3_mul_s(packet->e_velocity, t0);
		plane_intersect = ft_vec3_add(plane_intersect, tmp);
		if (point_in_triangle(plane_intersect, p1, p2, p3))
		{
			found_collision = TRUE;
			t = t0;
			colision_point = plane_intersect;
		}
	}

	if (!found_collision)
	{
		t_vec3 velocity = packet->e_velocity;
		t_vec3 base = packet->e_base_point;
		float velocity_sq_len = ft_vec3_dot(velocity, velocity);
		float a = velocity_sq_len;
		float b = 0;
		float c = 0;

		t_vec3 tmp = ft_vec3_sub(base, p1);
		b = 2.0 * ft_vec3_dot(velocity, tmp);
		tmp = ft_vec3_sub(p1, base);
		c = ft_vec3_dot(tmp, tmp) - 1;
		float new_t;
		if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
		{
			t = new_t;
			found_collision = TRUE;
			colision_point = p1;
		}

		// if (!found_collision)
		// {
			 tmp = ft_vec3_sub(base, p2);
			b = 2.0 * ft_vec3_dot(velocity, tmp);
			tmp = ft_vec3_sub(p2, base);
			c = ft_vec3_dot(tmp, tmp) - 1;
		//	float new_t;
			if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = p2;
			}
		// }

		// if (!found_collision)
		// {
			 tmp = ft_vec3_sub(base, p3);
			b = 2.0 * ft_vec3_dot(velocity, tmp);
			tmp = ft_vec3_sub(p3, base);
			c = ft_vec3_dot(tmp, tmp) - 1;
		//	float new_t;
			if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = p3;
			}
		// }

		t_vec3	edge = ft_vec3_sub(p2, p1);
		t_vec3	base_to_vertex = ft_vec3_sub(p1, base);
		float	edge_sq_len = ft_vec3_dot(edge, edge);
		float	edge_dot_velocity = ft_vec3_dot(edge, velocity);
		float	edge_dot_base_to_vertex = ft_vec3_dot(edge, base_to_vertex);

		a = edge_sq_len * -velocity_sq_len + edge_dot_velocity * edge_dot_velocity;
		b = edge_sq_len * (2. * ft_vec3_dot(velocity, base_to_vertex)) - 2. * edge_dot_velocity * edge_dot_base_to_vertex;
		c = edge_sq_len * (1. - ft_vec3_dot(base_to_vertex, base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;
		if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
		{
			float f = (edge_dot_velocity * new_t - edge_dot_velocity) / edge_sq_len;
			if (f >= 0 && f <= 1)
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = ft_vec3_add(p1, ft_vec3_mul_s(edge, f));
			}
		}

		edge = ft_vec3_sub(p3, p2);
		base_to_vertex = ft_vec3_sub(p2, base);
		edge_sq_len = ft_vec3_dot(edge, edge);
		edge_dot_velocity = ft_vec3_dot(edge, velocity);
		edge_dot_base_to_vertex = ft_vec3_dot(edge, base_to_vertex);

		a = edge_sq_len * -velocity_sq_len + edge_dot_velocity * edge_dot_velocity;
		b = edge_sq_len * (2. * ft_vec3_dot(velocity, base_to_vertex)) - 2. * edge_dot_velocity * edge_dot_base_to_vertex;
		c = edge_sq_len * (1. - ft_vec3_dot(base_to_vertex, base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;
		if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
		{
			float f = (edge_dot_velocity * new_t - edge_dot_velocity) / edge_sq_len;
			if (f >= 0 && f <= 1)
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = ft_vec3_add(p2, ft_vec3_mul_s(edge, f));
			}
		}

		edge = ft_vec3_sub(p1, p3);
		base_to_vertex = ft_vec3_sub(p3, base);
		edge_sq_len = ft_vec3_dot(edge, edge);
		edge_dot_velocity = ft_vec3_dot(edge, velocity);
		edge_dot_base_to_vertex = ft_vec3_dot(edge, base_to_vertex);

		a = edge_sq_len * -velocity_sq_len + edge_dot_velocity * edge_dot_velocity;
		b = edge_sq_len * (2. * ft_vec3_dot(velocity, base_to_vertex)) - 2. * edge_dot_velocity * edge_dot_base_to_vertex;
		c = edge_sq_len * (1. - ft_vec3_dot(base_to_vertex, base_to_vertex)) + edge_dot_base_to_vertex * edge_dot_base_to_vertex;
		if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
		{
			float f = (edge_dot_velocity * new_t - edge_dot_velocity) / edge_sq_len;
			if (f >= 0 && f <= 1)
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = ft_vec3_add(p3, ft_vec3_mul_s(edge, f));
			}
		}
	}

	if (found_collision)
	{
		float dist_to_coll = t * ft_vec3_len(packet->e_velocity);
		if (!packet->found_colision || dist_to_coll < packet->distance)
		{
			packet->distance = dist_to_coll;
			packet->intersect_point = colision_point;
			packet->found_colision = TRUE;
		}

		// t_vec3 n = ft_vec3_norm(ft_vec3_sub(colision_point, packet->e_velocity));
		// float dz = ft_vec3_dot(n, (t_vec3){ 0, 1, 0 });
		// if (dz <= -0.5)
		// 	packet->grounded = TRUE;
	}
	return (packet);
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

t_vec3		collide_with_world(t_physics_data *packet, t_vec3 e_position, t_vec3 e_velocity, float slide_threshold)
{
	float	unit_scale = 1;
	float	very_close_dist = 0.0000005f * unit_scale;

	if (packet->depth > 5)
	{
		//printf("RIP\n");
		return (e_position);
	}
	packet->e_velocity = e_velocity;
	packet->e_norm_velocity = ft_vec3_norm(e_velocity);
	packet->e_base_point = e_position;
	packet->found_colision = FALSE;
	packet->distance = INT_MAX;

	float scale = fmaxf(1.5, ft_vec3_len(e_velocity)) * 1.1;

	t_vec3 r3_position = ft_vec3_mul(e_position, packet->e_radius);
	/*
	ZONE MAXIMALE que le joueur peut atteindre en 1 mouvement 
	Possibilites d optimisations 
	t_vec3 query_radius = ft_vec3_mul_s(packet->e_radius, scale);
	t_vec3 min = ft_vec3_sub(r3_position, query_radius);
	t_vec3 max = ft_vec3_add(r3_position, query_radius);
	*/

	int i = -1;
	while (++i < packet->doom->renderables->len)
	{
		int j = -1;
		t_renderable r = packet->doom->renderables->values[i];
		while (++j < r.faces->len)
		{
			t_face face = r.faces->values[j];
			check_triangle(packet, 
				ft_vec3_div(vec4_to_3(r.pp_vertices[face.vertices_index[0] - 1]), packet->e_radius),
				ft_vec3_div(vec4_to_3(r.pp_vertices[face.vertices_index[1] - 1]), packet->e_radius),
				ft_vec3_div(vec4_to_3(r.pp_vertices[face.vertices_index[2] - 1]), packet->e_radius)
			);
		}
	}

	t_vec3	dest_point = ft_vec3_add(e_position, e_velocity);

	if (!packet->found_colision)
	{
	//	printf("NO_COLLISION GO! %f %f %f - %f %f %f\n", e_position.x, e_position.y, e_position.z, e_velocity.x, e_velocity.y, e_velocity.z);
		return (dest_point);
	}
	t_vec3	new_base_point = e_position;

	if (packet->distance >= very_close_dist)
	{
		t_vec3 v = ft_vec3_trim(e_velocity, packet->distance - very_close_dist);
		v = ft_vec3_norm(v);
		packet->intersect_point = ft_vec3_sub(packet->intersect_point, ft_vec3_mul_s(v, very_close_dist));
	}

	t_vec3	slide_plane_origin = packet->intersect_point;
	t_vec3	slide_plane_normal = ft_vec3_sub(new_base_point, packet->intersect_point);
	slide_plane_normal = ft_vec3_norm(slide_plane_normal); //TODO: Can probably be avoided by giving the plane normal with the colision detection

	t_plane	sliding_plane = plane_new(slide_plane_origin, slide_plane_normal);
	float	slide_factor = distance_to_plane(sliding_plane, dest_point);

	if (packet->intersect_point.y <= e_position.y - packet->e_radius.y + 0.1f && e_velocity.y <= 0)
		packet->grounded = TRUE;	
	t_vec3 new_dest_point = ft_vec3_sub(dest_point, ft_vec3_mul_s(slide_plane_normal, slide_factor));
	t_vec3 new_velocity = ft_vec3_sub(new_dest_point, packet->intersect_point);

	if (ft_vec3_len(new_velocity) < very_close_dist)
		return (new_base_point);
	packet->depth++;

	return (collide_with_world(packet, new_base_point, new_velocity, slide_threshold));
}

void		collide_and_slide(t_physics_data *packet, t_vec3 gravity)
{
	t_vec3	e_position = ft_vec3_div(packet->r3_posision, packet->e_radius);
	t_vec3	e_velocity = ft_vec3_div(packet->r3_velocity, packet->e_radius);
	t_vec3	final_position = (t_vec3){0, 0, 0};
	

	e_velocity.y = 0;
	float slide_threshold = 0.2;
	packet->depth = 0;
	t_bool grounded = packet->grounded;
	final_position = collide_with_world(packet, e_position, e_velocity, slide_threshold);
	packet->grounded = grounded;

	packet->r3_posision = ft_vec3_mul(final_position, packet->e_radius);
	packet->r3_velocity = gravity;

	e_velocity = ft_vec3_div(gravity, packet->e_radius);

	packet->depth = 0;
	final_position = collide_with_world(packet, final_position, e_velocity, slide_threshold);

	packet->r3_velocity = e_velocity;
	packet->r3_posision = ft_vec3_mul(final_position, packet->e_radius);
}