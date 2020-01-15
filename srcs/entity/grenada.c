/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grenada.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 14:02:33 by rle-ru            #+#    #+#             */
/*   Updated: 2020/01/14 15:34:22 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "entity.h"

t_bool	create_grenada(t_renderable *r, t_doom *doom)
{
	t_entity_grenada	*grenada;
	
	r->of.type = RENDERABLE_ENTITY;
	if (!(r->of.data.entity = ft_memalloc(sizeof(t_entity))))
		return (FALSE);
	r->of.data.entity->type = ENTITY_GRENADA;
	r->of.data.entity->packet.doom = doom;
	r->visible = TRUE;
	grenada = &r->of.data.entity->of.grenada;
	grenada->damage = 5;
	grenada->delay = 5000;
	grenada->range = 10;
	
	r->scale = (t_vec3){ 0.10, 0.10, 0.10 };
	r->of.data.entity->radius = (t_vec3){ 0.5, 0.5, 0.5 };
	//r->show_hitbox = TRUE;
	//compute_ellipsoid_hitbox(r, r->of.data.entity->position, r->of.data.entity->radius);
	return (TRUE);
}
