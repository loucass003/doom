/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 13:55:27 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/20 17:24:50 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "maths/quat.h"
#include "maths/vec4.h"
#include <math.h>

t_quat		quat_identity(void)
{
	return ((t_quat){ 0, 0, 0, 1 });
}

t_quat		quat_normalize(t_quat q)
{
	const t_vec4	v = ft_vec4_norm((t_vec4){ q.x, q.y, q.z, q.w });

	return ((t_quat){ v.x, v.y, v.z, v.w });
}

t_quat		quat_rotation_to(t_vec3 from, t_vec3 to)
{
	const float		dot = ft_vec3_dot(from, to);
	t_quat			quat;
	float			sd2;
	float			isd2;
	t_vec3			c;

	if (dot >= 1)
		return (quat_identity());
	if (dot < -1 + 1e-6)
	{
		quat = (t_quat){ to.y, -to.x, 0, 0 };
		if (quat.x * quat.x + quat.y * quat.y == 0)
			quat = (t_quat){ 0, to.z, -to.y, 0 };
	}
	else
	{
		sd2 = sqrt((1.0 + dot) * 2.0);
		isd2 = 1.0 / sd2;
		c = (t_vec3){ .x = from.x * to.z - from.z * to.y,
			.y = from.z * to.x - from.x * to.z,
			.z = from.x * to.y - from.y * to.x
		};
		quat = (t_quat){ c.x * isd2, c.y * isd2, c.z * isd2, sd2 * 0.5 };
	}
	return (quat_normalize(quat));
}

t_mat4		quat_to_matrix(t_quat q)
{
	t_mat4	m;
	t_matr	mat;

	mat.a = q.w;
	mat.b = q.x;
	mat.c = q.y;
	mat.d = q.z;
	mat.a2 = mat.a * mat.a;
	mat.b2 = mat.b * mat.b;
	mat.c2 = mat.c * mat.c;
	mat.d2 = mat.d;
	m.a[0][0] = mat.a2 + mat.b2 - mat.c2 - mat.d2;
	m.a[1][0] = 2.f * (mat.b * mat.c + mat.a * mat.d);
	m.a[2][0] = 2.f * (mat.b * mat.d - mat.a * mat.c);
	m.a[3][0] = 0.f;
	m.a[0][1] = 2 * (mat.b * mat.c - mat.a * mat.d);
	m.a[1][1] = mat.a2 - mat.b2 + mat.c2 - mat.d2;
	m.a[2][1] = 2.f * (mat.c * mat.d + mat.a * mat.b);
	m.a[3][1] = 0.f;
	m.a[0][2] = 2.f * (mat.b * mat.d + mat.a * mat.c);
	m.a[1][2] = 2.f * (mat.c * mat.d - mat.a * mat.b);
	m.a[2][2] = mat.a2 - mat.b2 - mat.c2 + mat.d2;
	m.a[3][2] = 0.f;
	m.a[0][3] = m.a[1][3] = m.a[2][3] = 0.f;
	m.a[3][3] = 1.f;
	return (m);
}
