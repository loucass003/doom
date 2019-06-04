/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/06/04 23:25:23 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"



void	update_maxtrix(t_doom *doom)
{
	doom->player.matrix = ft_mat4_mul(
		doom->player.projection, 
		ft_mat4_mul(
			ft_mat4_rotation((t_vec3){0, -doom->player.rotation, 0}),
			ft_mat4_translation(ft_vec3_inv(doom->player.pos))
		)
	);
	/*doom->player.matrix = ft_mat4_mul(
		ft_mat4_rotation((t_vec3){0, -doom->player.rotation, 0}),
		ft_mat4_translation(ft_vec3_inv(doom->player.pos))
	);*/
}

t_mat4	projection_matrix(t_doom *doom)
{
	const float	a = (FAR + NEAR) / (FAR - NEAR);
	const float	b = -(2 * FAR * NEAR) / (FAR - NEAR);

	return ((t_mat4)((t_mat4_data) {
		1. / (S_HEIGHT / S_WIDTH), 0, 0, 0,
		0, 1, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0}));
}
