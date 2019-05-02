/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/02 13:29:48 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define FOV (60 * (M_PI / 180))

void	update_maxtrix(t_doom *doom)
{
	doom->player.matrix = ft_mat4_mul(
		doom->player.projection,
		ft_mat4_mul(
			ft_mat4_rotation((t_vec3){0, doom->player.rotation, 0}),
			ft_mat4_translation(doom->player.pos)
		)
	);
}

t_mat4	projection_matrix(t_doom *doom)
{
	const float far = 100;
	const float near = 0.1;
	const float	a = (far + near) / (near - far);
	const float	b = (2 * far * near) / (near - far);

	return ((t_mat4)((t_mat4_data) {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, a, -1,
		0, 0, b, 0}));
}
