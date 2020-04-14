/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 17:50:27 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 19:13:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <math.h>
#include "maths/vec4.h"

t_vec4	vec3_to_4(t_vec3 v)
{
	return ((t_vec4){v.x, v.y, v.z, 1});
}

t_vec3	vec4_to_3w(t_vec4 v)
{
	return ((t_vec3){v.x / v.w, v.y / v.w, v.z / v.w});
}

t_vec3	vec4_to_3(t_vec4 v)
{
	return ((t_vec3){v.x, v.y, v.z});
}
