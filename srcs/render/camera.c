/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 23:57:39 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/02 01:35:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define FOV (60 * (M_PI / 180))

void	update_maxtrix(t_doom *doom)
{
	doom->player.matrix = ft_mat4_mul(
		ft_mat4_rotation((t_vec3){0, doom->player.rotation, 0}),
		doom->player.projection
	);
}

t_mat4	projection_matrix(t_doom *doom)
{
	/*const float		ar = doom->screen.width / doom->screen.height;
	const float		c = tanf(FOV * .5);
	const float		range = 0.1 - 20;

	return ((t_mat4)((t_mat4_data){
		-1. / (c * ar), 0, 0, 0,
		0, 1 / c, 0, 0,
		0, 0, (-0.1 - 20) / range, 2 * 0.1 * 20 / range ,
		0, 0, 1, 0
	}));*/

	const float	scale = 1. / tanf(60 * 0.5 * M_PI / 180);
	const float	a = -100. / (100. - 0.1);
	const float	b = -100. * 0.1 / (100. - 0.1);

	return ((t_mat4)((t_mat4_data) {
		-scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, a, b,
		0, 0, -1, 0}));
}