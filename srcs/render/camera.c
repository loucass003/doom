/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/17 16:59:27 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	update_maxtrix(t_doom *doom)
{
	doom->player.obb = (t_obb_box){
		.pos = doom->player.pos,
		.half_size = {0.1, 0.1, 0.1},
		.x_axis = {1, 0, 0},
		.y_axis = {0, 1, 0},
		.z_axis = {0, 0, 1}
	};
	/* doom->player.matrix = ft_mat4_mul(
		doom->player.projection, 
		ft_mat4_mul(
			ft_mat4_rotation((t_vec3){0, -doom->player.rotation, 0}),
			ft_mat4_translation(ft_vec3_inv(doom->player.pos))
		)
	); */
	doom->player.matrix = ft_mat4_mul(
		ft_mat4_rotation((t_vec3){doom->player.rotation.x, -doom->player.rotation.y, 0}),
		ft_mat4_translation(ft_vec3_inv(doom->player.pos))
	);
}

t_mat4	projection_matrix(t_doom *doom)
{
	const float	a = (FAR + NEAR) / (FAR - NEAR);
	const float	b = -(2 * FAR * NEAR) / (FAR - NEAR);

	return ((t_mat4)((t_mat4_data) {
		ASPECT_INV, 0, 0, 0,
		0, ASPECT_INV, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0}));
}
