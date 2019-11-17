/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/15 23:50:58 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/17 01:10:42 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <limits.h>
#include "maths/triangle.h"
#include "maths/plane.h"
#include "doom.h"
#include "collision.h"

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
	return (fmin(max, fmax(min, v)));
}

t_physics_data		*check_triangle(t_renderable *r, t_physics_data *packet, t_vec3 p1, t_vec3 p2, t_vec3 p3)
{
	const	t_plane plane = triangle_to_plane(p1, p2, p3);
	
	packet->r = NULL;
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

		if (!found_collision)
		{
			tmp = ft_vec3_sub(base, p2);
			b = 2.0 * ft_vec3_dot(velocity, tmp);
			tmp = ft_vec3_sub(p2, base);
			c = ft_vec3_dot(tmp, tmp) - 1;
			if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = p2;
			}
		}

		if (!found_collision)
		{
			tmp = ft_vec3_sub(base, p3);
			b = 2.0 * ft_vec3_dot(velocity, tmp);
			tmp = ft_vec3_sub(p3, base);
			c = ft_vec3_dot(tmp, tmp) - 1;
			if (lowest_root((t_vec3){ a, b, c }, t, &new_t))
			{
				t = new_t;
				found_collision = TRUE;
				colision_point = p3;
			}
		}

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
			float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_sq_len;
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
			float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_sq_len;
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
			float f = (edge_dot_velocity * new_t - edge_dot_base_to_vertex) / edge_sq_len;
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
		//	printf("COLLISION\n");
			packet->distance = dist_to_coll;
			packet->intersect_point = colision_point;
			packet->found_colision = TRUE;
			packet->r = r;
			// packet->t = t;
			// packet->a = p1;
			// packet->b = p2;
			// packet->c = p3;
			// packet->plane = plane;
		}

		t_vec3 n = ft_vec3_norm(ft_vec3_sub(colision_point, packet->e_velocity));
		float dz = ft_vec3_dot(n, (t_vec3){ 0, 1, 0 });
		packet->grounded = dz <= -0.5;
		
	}
	// if (!found_collision)
	// 	printf("NOT COLLIDE\n");
	return (packet);
}
