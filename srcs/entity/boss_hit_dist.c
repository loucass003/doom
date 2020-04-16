/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boss_hit_dist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 17:21:55 by lloncham          #+#    #+#             */
/*   Updated: 2020/04/16 17:29:09 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include <math.h>

void		boss_shoot(t_doom *doom, t_entity *entity, t_entity_boss *boss)
{
	t_vec3 pos;
	t_mat4 rot;
	t_vec3 p_left_right;

	pos = entity->position;
	rot = ft_mat4_rotation(entity->rotation);
	if (boss->shoot % 2 == 0)
	{
		p_left_right = ft_mat4_mulv(rot, (t_vec3){ 0, 0, 0.4 * 5 });
		p_left_right = ft_vec3_add(p_left_right, pos);
		renderable_rocket(doom, p_left_right, doom->player.camera.pos);
	}
	else
	{
		p_left_right = ft_mat4_mulv(rot, (t_vec3){ 0, 0, -0.4 * 5 });
		p_left_right = ft_vec3_add(p_left_right, pos);
		renderable_rocket(doom, p_left_right, doom->player.camera.pos);
	}
}

void		boss_shoot_true(t_doom *doom, t_entity *entity, t_entity_boss *boss)
{
	boss->t2 = 0;
	boss->animation_step++;
	boss->shoot++;
	if (boss->animation_step >= 7)
	{
		boss->shoot = 0;
		boss->animation_step = 4;
		boss->shooting = FALSE;
	}
	entity_sound(entity, 5, 1, 1);
	boss_shoot(doom, entity, boss);
}

void		boss_hit_dist(t_doom *doom, t_entity *entity, t_entity_boss *boss)
{
	if (boss->shooting)
		boss->t1 = 0;
	boss->t1++;
	if (boss->t1 >= 5 && !boss->shooting)
	{
		boss->shooting = TRUE;
		boss->t1 = 0;
		boss->animation_step = 4;
	}
	if (boss->shooting)
	{
		boss->t2++;
		if (boss->t2 >= 3)
			boss_shoot_true(doom, entity, boss);
	}
	else
		boss->animation_step = 4;
}
