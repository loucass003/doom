/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_of_basis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 19:33:06 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 19:40:47 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "maths/mat4.h"

t_bool			compute_change_of_basis(t_vec3 n, t_mat4 *p_inv,
		t_mat4 *reverse)
{
	const t_vec3	up = (t_vec3){0, 0, 1};
	t_vec3			u;
	t_vec3			w;

	u = ft_vec3_cross(n, up);
	if (ft_vec3_len(u) == 0)
		u = (t_vec3){0, 1, 0};
	w = ft_vec3_cross(u, n);
	reverse->d = (t_mat4_data){
		u.x, w.x, n.x, 0,
		u.y, w.y, n.y, 0,
		u.z, w.z, n.z, 0,
		0, 0, 0, 1
	};
	return (mat4_inverse(*reverse, p_inv));
}
