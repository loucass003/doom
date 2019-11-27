/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frustum.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 23:35:31 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/27 17:07:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <maths/vec4.h>
#include <collision.h>

void		compute_frustum_planes(t_mat4 m, t_vec4 *planes)
{
	float me0 = m.b[ 0 ], me1 = m.b[ 1 ], me2 = m.b[ 2 ], me3 = m.b[ 3 ];
	float me4 = m.b[ 4 ], me5 = m.b[ 5 ], me6 = m.b[ 6 ], me7 = m.b[ 7 ];
	float me8 = m.b[ 8 ], me9 = m.b[ 9 ], me10 = m.b[ 10 ], me11 = m.b[ 11 ];
	float me12 = m.b[ 12 ], me13 = m.b[ 13 ], me14 = m.b[ 14 ], me15 = m.b[ 15 ];

	// t_vec4 r1 = (t_vec4){ m.b[0][0], m.a[0][1], m.a[0][2], m.a[0][3] };
	// t_vec4 r2 = (t_vec4){ m.a[1][0], m.a[1][1], m.a[1][2], m.a[1][3] };
	// t_vec4 r3 = (t_vec4){ m.a[2][0], m.a[2][1], m.a[2][2], m.a[2][3] };
	// t_vec4 r4 = (t_vec4){ m.a[3][0], m.a[3][1], m.a[3][2], m.a[3][3] };

	

	planes[0] = ft_vec4_norm((t_vec4){ me3 - me0, me7 - me4, me11 - me8, me15 - me12 });
	planes[1] = ft_vec4_norm((t_vec4){ me3 + me0, me7 + me4, me11 + me8, me15 + me12 });
	planes[2] = ft_vec4_norm((t_vec4){ me3 + me1, me7 + me5, me11 + me9, me15 + me13 });
	planes[3] = ft_vec4_norm((t_vec4){ me3 - me1, me7 - me5, me11 - me9, me15 - me13 });
	planes[4] = ft_vec4_norm((t_vec4){ me3 - me2, me7 - me6, me11 - me10, me15 - me14 });
	planes[5] = ft_vec4_norm((t_vec4){ me3 + me2, me7 + me6, me11 + me10, me15 + me14 });

	// planes[0] = ft_vec4_norm(ft_vec4_add(r4, r1));
	// planes[1] = ft_vec4_norm(ft_vec4_sub(r4, r1));
	// planes[2] = ft_vec4_norm(ft_vec4_add(r4, r2));
	// planes[3] = ft_vec4_norm(ft_vec4_sub(r4, r2));
	// planes[4] = ft_vec4_norm(ft_vec4_add(r4, r3));
	// planes[5] = ft_vec4_norm(ft_vec4_sub(r4, r3));

	// for (int i = 0; i < 6; i++)
	// 	printf("%f %f %f\n", planes[i].x, planes[i].y, planes[i].z);
}

t_bool		aabb_vs_frustrum(t_collide_aabb aabb, t_vec4 *planes)
{
	int		i;
	t_vec4	plane;
	t_vec3	p1;
	t_vec3	p2;

	i = -1;
	while (++i < 6)
	{
		plane = planes[i];
		p1.x = plane.x > 0 ? aabb.min.x : aabb.max.x;
		p2.x = plane.x > 0 ? aabb.max.x : aabb.min.x;
		p1.y = plane.y > 0 ? aabb.min.y : aabb.max.y;
		p2.y = plane.y > 0 ? aabb.max.y : aabb.min.y;
		p1.z = plane.z > 0 ? aabb.min.z : aabb.max.z;
		p2.z = plane.z > 0 ? aabb.max.z : aabb.min.z;
		if (ft_vec3_dot(p1, vec4_to_3(plane)) + plane.w < 0 
			&& ft_vec3_dot(p2, vec4_to_3(plane)) + plane.w < 0)
			return (FALSE);
	}
	return (TRUE);
}