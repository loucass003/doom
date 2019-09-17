/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 22:09:35 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/17 22:10:36 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "maths/triangle.h"
#include "maths/plane.h"

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

t_bool	is_front_facing(t_plane plane, t_vec3 direction)
{
	const float f = ft_vec3_dot(plane.normal, direction);

	return (f <= 0);
}
