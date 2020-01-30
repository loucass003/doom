/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:25:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/30 15:26:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths/mat4.h"

t_vec3	rotation_matrix_to_euler(t_mat4 m)
{
	float sy = sqrtf(m.a[0][0] * m.a[0][0] + m.a[0][1] * m.a[0][1]);
	t_bool singlular = sy < 1e-6;
	t_vec3 out;

	if (!singlular)
	{
		out.x = atan2(m.a[1][2], m.a[2][2]);
		out.y = atan2(-m.a[0][2], sy);
		out.z = atan2(m.a[0][1], m.a[0][0]);
	}
	else
	{
		out.x = atan2(-m.a[2][1], m.a[1][1]);
		out.y = atan2(-m.a[0][2], sy);
		out.z = 0;
	}
	return (out);	
}
