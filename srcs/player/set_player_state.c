/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_player_state.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/12 12:57:37 by lloncham          #+#    #+#             */
/*   Updated: 2020/03/12 13:06:07 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "player.h"

t_bool		aabb_intersect_world(t_doom *doom, t_collide_aabb aabb)
{
	t_collision		hit;
	t_renderable	*r;
	int				i;
	int				j;

	i = -1;
	while (++i < doom->renderables->len)
	{
		r = &doom->renderables->values[i];
		if (ray_skip_renderable(r))
			continue;
		if (r->has_hitbox && r->hitbox.type == COLLIDE_ELLIPSOID)
		{
			t_collide_ellipsoid	ellipsoid;
			t_renderable		*sphere;
			ellipsoid = r->hitbox.data.ellipsoid;
			sphere = &doom->sphere_primitive;
			sphere->position = ellipsoid.origin;
			sphere->scale = ellipsoid.radius;
			sphere->dirty = TRUE;
			transform_renderable(sphere);
			r = sphere;
		}
		j = -1;
		while (++j < r->faces->len)
		{
			if (!r->faces->values[j].has_collision && doom->main_context.type == CTX_NORMAL)
				continue;
			if (r->faces->values[j].face_normal.y >= -1e-6)
				continue;
			hit = triangle_hit_aabb(&r->faces->values[j].collidable.data.triangle, &aabb);
			if (hit.collide)
				return (TRUE);
		}
	}
	return (FALSE);
}

t_bool		set_player_height(t_doom *doom, t_player *player, float height)
{
	t_entity	*e;
	float		diff;

	e = &player->entity;

	diff = (height / 2.0) - e->radius.y;
	if (diff > 0)
	{
		t_vec3			pos;
		t_vec3			radius;
		t_collide_aabb 	aabb;
		pos = e->position;
		pos.y += height / 2;
		radius = (t_vec3){ e->radius.x, height / 2, e->radius.z };
		aabb = (t_collide_aabb){
			.min = ft_vec3_sub(pos, radius),
			.max = ft_vec3_add(pos, radius)
		};
		if (aabb_intersect_world(doom, aabb))
			return (FALSE);
	}
	e->radius.y = height / 2;
	if (diff < 0)
		e->position.y -= height / 2;
	if (diff > 0)
		e->position.y += height / 2;
	return (TRUE);
}

void		set_player_state(t_doom *doom, t_player *player, t_player_state state)
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
