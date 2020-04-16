/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_player.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:37:44 by Lisa              #+#    #+#             */
/*   Updated: 2020/04/16 17:38:19 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

t_bool				set_player_height(t_doom *doom, t_player *player,
	float height)
{
	t_entity		*e;
	float			diff;
	t_vec3			pos;
	t_vec3			radius;
	t_collide_aabb	aabb;

	e = &player->entity;
	diff = (height / 2.0) - e->radius.y;
	if (diff > 0)
	{
		pos = e->position;
		pos.y += height / 2;
		radius = (t_vec3){ e->radius.x, height / 2, e->radius.z };
		aabb = (t_collide_aabb){ .min = ft_vec3_sub(pos, radius),
			.max = ft_vec3_add(pos, radius)};
		if (aabb_intersect_world(doom, aabb))
			return (FALSE);
	}
	e->radius.y = height / 2;
	e->position.y = (diff < 0 ? e->position.y - (height / 2)
		: e->position.y + (height / 2));
	return (TRUE);
}

void				set_player_state(t_doom *doom, t_player *player,
	t_player_state state)
{
	player->desired_state = state;
	if (state == PS_CROUCH)
	{
		if (set_player_height(doom, player, 2.5))
			player->player_state = state;
	}
	else if (state == PS_NORMAL)
	{
		if (set_player_height(doom, player, 3 * 2))
			player->player_state = state;
	}
}
