/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_world.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/14 16:03:32 by louali            #+#    #+#             */
/*   Updated: 2020/04/15 00:24:22 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_vec3		editor_to_world(t_vec3 pos)
{
	float	ratio;

	ratio = 1. / 5.;
	pos.x *= ratio;
	pos.z *= ratio;
	return (pos);
}

t_vec3		world_to_editor(t_vec3 pos)
{
	float	ratio;

	ratio = 5;
	pos.x *= ratio;
	pos.z *= ratio;
	return (pos);
}
