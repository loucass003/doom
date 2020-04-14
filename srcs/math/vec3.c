/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:52:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:10:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>

t_vec3			vec3_clamp_s(t_vec3 v, float min, float max)
{
	v.x = fmax(min, fmin(max, v.x));
	v.y = fmax(min, fmin(max, v.y));
	v.z = fmax(min, fmin(max, v.z));
	return (v);
}

t_vec3			vec3_add_s(t_vec3 v, float s)
{
	v.x += s;
	v.y += s;
	v.z += s;
	return (v);
}

t_vec3			vec3_sub_s(t_vec3 v, float s)
{
	v.x -= s;
	v.y -= s;
	v.z -= s;
	return (v);
}

t_vec3			ft_vec3_interpolate(t_vec3 a, t_vec3 b, float alpha)
{
	return (ft_vec3_add(a, ft_vec3_mul_s(ft_vec3_sub(b, a), alpha)));
}
