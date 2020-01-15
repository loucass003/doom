/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/13 14:28:15 by lloncham         ###   ########.fr       */
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
	camera->forward = (t_vec3){ camera->matrix.a[2][0], camera->matrix.a[2][1], camera->matrix.a[2][2] };
	camera->frustum_matrix = ft_mat4_mul(
		camera->projection,
		ft_mat4_mul(
			ft_mat4_rotation((t_vec3){-camera->rotation.x, -camera->rotation.y - M_PI, -camera->rotation.z}),
			ft_mat4_translation((t_vec3){-camera->pos.x, -camera->pos.y, -camera->pos.z})
		)
	);
}

t_mat4	look_at(t_vec3 from, t_vec3 to)
{
	const t_vec3 forward = ft_vec3_norm(ft_vec3_sub(from, to));
	const t_vec3 right = ft_vec3_cross((t_vec3){0, 1, 0}, forward);
	const t_vec3 up = ft_vec3_cross(forward, right);

	t_mat4 mat;

	mat.a[0][0] = right.x;
	mat.a[0][1] = right.y;
	mat.a[0][2] = right.z;
	mat.a[1][0] = up.x;
	mat.a[1][1] = up.y;
	mat.a[1][2] = up.z;
	mat.a[2][0] = forward.x;
	mat.a[2][1] = forward.y;
	mat.a[2][2] = forward.z;

	mat.a[3][0] = from.x;
	mat.a[3][1] = from.y;
	mat.a[3][2] = from.z;
	return (mat);
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
