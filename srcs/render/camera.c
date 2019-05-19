/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/19 23:22:18 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define FOV (60 * (M_PI / 180))

void	update_maxtrix(t_doom *doom)
{
	doom->player.matrix = ft_mat4_mul(
		doom->player.projection, 
		ft_mat4_mul(
			ft_mat4_rotation((t_vec3){0, -doom->player.rotation, 0}),
			ft_mat4_translation(ft_vec3_inv(doom->player.pos))
		)
	);
}

t_mat4	projection_matrix(t_doom *doom)
{
	const float far = 100;
	const float near = 0.1;
	const float	a = -(far + near) / (far - near);
	const float	b = -(2 * far * near) / (far - near);

	return ((t_mat4)((t_mat4_data) {
		1. / (S_HEIGHT / S_WIDTH), 0, 0, 0,
		0, 1. / (S_HEIGHT / S_WIDTH), 0, 0,
		0, 0, a, b,
		0, 0, -1, 0}));
}
