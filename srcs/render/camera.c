/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/11/28 04:02:45 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "maths/mat4.h"

void	camera_update_maxtrix(t_camera *camera)
{
	camera->matrix = ft_mat4_mul(
		ft_mat4_rotation((t_vec3){camera->rotation.x, -camera->rotation.y, camera->rotation.z}),
		ft_mat4_translation(ft_vec3_inv(camera->pos))
	);
	camera->total_matrix = ft_mat4_mul(camera->projection, camera->matrix);
	// t_mat4 p = camera->projection;
	// //p.b[10] *= -1;
	// t_mat4 m = ft_mat4_mul(
	// 	camera->projection,
	// 	ft_mat4_mul(
	// 		ft_mat4_rotation((t_vec3){-camera->rotation.x, -camera->rotation.y - M_PI, -camera->rotation.z}),
	// 		ft_mat4_translation((t_vec3){-camera->pos.x, -camera->pos.y, -camera->pos.z})
	// 	)
	// );
	// compute_frustum_planes(m, camera->frustum);
}

t_mat4	projection_matrix()
{
	const float	a = ((FAR + NEAR) / (FAR - NEAR));
	const float	b = (2 * FAR * NEAR) / (FAR - NEAR);
	const float c = (1. / ((float)(S_WIDTH / S_HEIGHT) * tan(FOV / 2.)));
	const float d = (1. / tan(FOV / 2.));

	return((t_mat4)((t_mat4_data) {
		c, 0, 0, 0,
		0, d, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0}));
}
