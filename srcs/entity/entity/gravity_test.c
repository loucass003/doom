/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gravity_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 01:24:50 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/21 01:28:20 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"

t_bool		gravity_test(t_entity *e, t_vec3 gravity, t_vec3 final_pos)
{
	t_vec3	e_velocity;
	t_bool	stop;

	if (e->type == ENTITY_PLAYER || gravity.y != 0)
	{
		e->packet.e_velocity = gravity;
		e_velocity = ft_vec3_div(gravity, e->packet.e_radius);
		e->packet.depth = -1;
		stop = FALSE;
		final_pos = collide_with_world(e, final_pos, e_velocity, &stop);
		if (stop)
			return (FALSE);
	}
	e->packet.r3_posision = ft_vec3_mul(final_pos, e->packet.e_radius);
	return (TRUE);
}
