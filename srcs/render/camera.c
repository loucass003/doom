/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/09/26 15:50:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	camera_update_maxtrix(t_camera *camera)
{
	camera->matrix = ft_mat4_mul(
		ft_mat4_rotation((t_vec3){camera->rotation.x, -camera->rotation.y, camera->rotation.z}),
		ft_mat4_translation(ft_vec3_inv(camera->pos))
	);
}

t_mat4	projection_matrix()
{
	const float	a = (FAR + NEAR) / (FAR - NEAR);
	const float	b = -(2 * FAR * NEAR) / (FAR - NEAR);

	return ((t_mat4)((t_mat4_data) {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0}));
}
