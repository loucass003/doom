/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_add_sub.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 19:11:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:11:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "maths/vec4.h"

t_vec4	ft_vec4_sub(t_vec4 a, t_vec4 b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return (a);
}

t_vec4	ft_vec4_sub_s(t_vec4 a, float v)
{
	a.x -= v;
	a.y -= v;
	a.z -= v;
	a.w -= v;
	return (a);
}

t_vec4	ft_vec4_add(t_vec4 a, t_vec4 b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return (a);
}
