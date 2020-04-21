/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vec3_norm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 18:09:15 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/20 14:22:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

// static float	q_rsqrt(float number)
// {
// 	long		i;
// 	float		x2;
// 	float		y;

// 	x2 = number * 0.5F;
// 	y = number;
// 	i = *(long *)&y;
// 	i = 0x5f3759df - (i >> 1);
// 	y = *(float *)&i;
// 	y = y * (1.5F - (x2 * y * y));
// 	return (y);
// }

t_vec3			ft_vec3_norm(t_vec3 v)
{
	const float	len = 1. / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return ((t_vec3) { v.x * len, v.y * len, v.z * len });
}
