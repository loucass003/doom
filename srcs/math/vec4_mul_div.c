/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_mul_div.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:10:57 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:12:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "maths/vec4.h"

t_vec4	ft_vec4_mul_s(t_vec4 a, float v)
{
	a.x *= v;
	a.y *= v;
	a.z *= v;
	a.w *= v;
	return (a);
}

t_vec4	ft_vec4_div_s(t_vec4 a, float v)
{
	a.x /= v;
	a.y /= v;
	a.z /= v;
	a.w /= v;
	return (a);
}

t_vec4	ft_vec4_div(t_vec4 a, t_vec4 b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;
	a.w /= b.w;
	return (a);
}

t_vec4	ft_vec4_mul(t_vec4 a, t_vec4 b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;
	a.w *= b.w;
	return (a);
}
