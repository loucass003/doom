/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 17:34:17 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 17:35:19 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "entity.h"
#include "doom.h"

t_bool		damage_entity(t_entity *from, float dist,
	float damage, t_renderable *entity)
{
	t_entity	*e;

	e = entity->of.data.entity;
	if (from == e)
		return (TRUE);
	dist = ft_vec3_len(ft_vec3_sub(e->position, from->position));
	if (dist < 10)
	{
		damage = from->of.rocket.damage;
		if (dist != 0)
			damage /= dist;
		give_damage(from, e, e->packet.doom, damage);
	}
	return (FALSE);
}

void		damage_explo(t_entity *from,
	t_doom *doom, float damage)
{
	t_renderables	*renderables;
	float			dist;
	int				i;
	t_renderable	r;
	t_renderable	*entity;

	dist = 0;
	create_explosion_renderable(doom, &r);
	r.position = from->position;
	append_renderables_array(&doom->renderables, r);
	i = -1;
	renderables = doom->renderables;
	while (++i < renderables->len)
	{
		entity = &renderables->values[i];
		if (entity->of.type == RENDERABLE_ENTITY)
			if (damage_entity(from, dist, damage, entity))
				continue;
	}
}

void		give_damage(t_entity *from, t_entity *to,
	t_doom *doom, float damage)
{
	float d;

	d = damage;
	if (from && from->type == ENTITY_PLAYER)
		d = damage * doom->level.coeff_damage;
	to->life -= d;
	if (from && (from->type == ENTITY_PLAYER || from->type == ENTITY_GRENADA)
		&& to->type != ENTITY_PLAYER && to->killable
		&& !to->diying && !to->dead)
	{
		doom->gameover.totaldamage += d;
		if (to->life <= 0)
		{
			to->diying = TRUE;
			doom->gameover.kill += 1;
		}
	}
	if (to->type == ENTITY_BOSS)
		entity_sound(to, 7, 3, 1);
}
