/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:25:31 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:01:50 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "maths/mat4.h"
#include <math.h>

t_vec3	rotation_matrix_to_euler(t_mat4 m)
{
	float	sy;
	t_bool	singlular;
	t_vec3	out;

	sy = sqrtf(m.a[0][0] * m.a[0][0] + m.a[0][1] * m.a[0][1]);
	singlular = sy < 1e-6;
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

t_mat4	mat4_transpose(t_mat4 a)
{
	int		i;
	int		j;
	t_mat4	m;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			m.a[i][j] = a.a[j][i];
	}
	return (m);
}
