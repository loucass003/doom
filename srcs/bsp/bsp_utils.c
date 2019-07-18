/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/13 06:59:58 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/13 07:06:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_node	*get_player_node(t_node *bsp, t_vec3 pos)
{
	const t_vec2	p = (t_vec2){pos.x, pos.z};
	t_node			*n;

	n = bsp;
	while (n->type != N_LEAF)
	{
		if (get_side_thin(n->partition, p) == S_FRONT)
			n = n->front;
		else
			n = n->back;
	}
	return (n);
}