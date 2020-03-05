/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 08:06:13 by llelievr          #+#    #+#             */
/*   Updated: 2020/03/05 19:05:38 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "maths.h"
#include "collision.h"

t_bool		get_separating_plane(t_vec3 pos, t_vec3 plane, t_obb_box a,
	t_obb_box b)
{
	return (ft_abs(ft_vec3_dot(pos, plane))
		> (ft_abs(ft_vec3_dot(ft_vec3_mul_s(a.x_axis, a.half_size.x), plane)))
		+ (ft_abs(ft_vec3_dot(ft_vec3_mul_s(a.y_axis, a.half_size.y), plane)))
		+ (ft_abs(ft_vec3_dot(ft_vec3_mul_s(a.z_axis, a.half_size.z), plane)))
		+ (ft_abs(ft_vec3_dot(ft_vec3_mul_s(b.x_axis, b.half_size.x), plane)))
		+ (ft_abs(ft_vec3_dot(ft_vec3_mul_s(b.y_axis, b.half_size.y), plane)))
		+ (ft_abs(ft_vec3_dot(ft_vec3_mul_s(b.z_axis, b.half_size.z), plane))));
}

t_bool		get_obb_collision(t_obb_box a, t_obb_box b)
{
	const	t_vec3	pos = ft_vec3_sub(b.pos, a.pos);

	return (!((get_separating_plane(pos, a.x_axis, a, b)
		|| get_separating_plane(pos, a.y_axis, a, b)
		|| get_separating_plane(pos, a.z_axis, a, b)
		|| get_separating_plane(pos, b.x_axis, a, b)
		|| get_separating_plane(pos, b.y_axis, a, b)
		|| get_separating_plane(pos, b.z_axis, a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.x_axis, b.x_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.x_axis, b.y_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.x_axis, b.z_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.y_axis, b.x_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.y_axis, b.y_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.y_axis, b.z_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.z_axis, b.x_axis), a, b)
		|| get_separating_plane(pos, ft_vec3_cross(a.z_axis, b.y_axis), a, b)
		|| get_separating_plane(pos,
			ft_vec3_cross(a.z_axis, b.z_axis), a, b))));
}
