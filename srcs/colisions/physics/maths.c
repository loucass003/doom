/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 21:48:51 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 21:49:01 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "collision.h"

void				edge_collision(t_physics_fields *f,
	t_physics_data *pa, t_vec3 p[2])
{
	t_vec3	edge;
	t_vec3	b_to_v;
	float	e[3];
	float	i;
	float	new_t;

	edge = ft_vec3_sub(p[0], p[1]);
	b_to_v = ft_vec3_sub(p[1], pa->e_base_point);
	e[0] = ft_vec3_dot(edge, edge);
	e[1] = ft_vec3_dot(edge, pa->e_velocity);
	e[2] = ft_vec3_dot(edge, b_to_v);
	f->a[0] = e[0] * -f->velocity_sq_len + e[1] * e[1];
	f->a[1] = e[0] * (2. * ft_vec3_dot(pa->e_velocity, b_to_v)) - 2.
		* e[1] * e[2];
	f->a[2] = e[0] * (1. - ft_vec3_dot(b_to_v, b_to_v)) + e[2] * e[2];
	if (lowest_root((t_vec3){ f->a[0], f->a[1], f->a[2] }, f->t, &new_t))
	{
		i = (e[1] * new_t - e[2]) / e[0];
		if (i >= 0 && i <= 1)
		{
			f->t = new_t;
			f->found_collision = TRUE;
			f->colision_point = ft_vec3_add(p[1], ft_vec3_mul_s(edge, i));
		}
	}
}

void				lowest_collision(t_physics_fields *fields,
	t_physics_data *packet, t_vec3 p)
{
	t_vec3	tmp;
	float	new_t;

	if (!fields->found_collision)
	{
		tmp = ft_vec3_sub(packet->e_base_point, p);
		fields->a[1] = 2.0 * ft_vec3_dot(packet->e_velocity, tmp);
		tmp = ft_vec3_sub(p, packet->e_base_point);
		fields->a[2] = ft_vec3_dot(tmp, tmp) - 1;
		if (lowest_root((t_vec3){ fields->a[0], fields->a[1], fields->a[2] },
			fields->t, &new_t))
		{
			fields->t = new_t;
			fields->found_collision = TRUE;
			fields->colision_point = p;
		}
	}
}
