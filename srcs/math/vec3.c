/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 18:52:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/08/27 18:54:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>

t_vec3		vec3_clamp_s(t_vec3 v, float min, float max)
{
	v.x = fmax(min, fmin(max, v.x));
	v.y = fmax(min, fmin(max, v.y));
	v.z = fmax(min, fmin(max, v.z));
	return v;
}
