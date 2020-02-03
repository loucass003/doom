/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grenada.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 14:02:33 by rle-ru            #+#    #+#             */
/*   Updated: 2020/02/03 15:23:10 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "doom.h"
#include "render.h"
#include "sprite.h"
#include "entity.h"
#include "explosion.h"
#include <stdlib.h>
#include <SDL.h>

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
	grenada->damage = 5 * doom->level.coeff_damage;
	grenada->delay = 2000;
	grenada->range = 10;
	r->scale = (t_vec3){ 0.10, 0.10, 0.10 };
	r->of.data.entity->radius = (t_vec3){ 0.5, 0.5, 0.5 };
	return (TRUE);
}

t_bool		create_explosion_renderable(t_doom *doom, t_renderable *r)
{
	t_sprite	*sprite;

	if (!(sprite = create_sprite((t_vec2){ 8, 6 }, doom->res_manager.ressources->values[24])))
		return (FALSE);
	if (!create_sprite_renderable(r, sprite))
		return (FALSE);
	r->of.type = RENDERABLE_EXPLOSION;
	if (!(r->of.data.explosion = ft_memalloc(sizeof(t_explosion))))
		return (FALSE);
	r->scale = (t_vec3){ 2, 2, 2 };
	set_current_cell(r, 0, 0);
	return (TRUE);
}

int			renderable_from_entity(t_renderables *renderables, t_entity *entity)
{
	int		i;

	i = -1;
	while (++i < renderables->len)
	{
		if (renderables->values[i].of.type == RENDERABLE_ENTITY 
			&& renderables->values[i].of.data.entity == entity)
			return (i);
	}
	return (-1);
}

t_bool		entity_update_grenada(t_doom *doom, t_entity *entity, double dt)
{
	t_entity_grenada *g;

	g = &entity->of.grenada;
	if (SDL_GetTicks() - g->start > g->delay)
	{
		damage_explo(entity, doom, g->damage);
		splice_renderables_array(doom->renderables, renderable_from_entity(doom->renderables, entity), 1);
		return (FALSE);
	}
	return (TRUE);
}


t_bool		update_renderable_explosion(t_render_context *ctx, t_renderable *r)
{
	t_explosion	*e;

	e = r->of.data.explosion;
	e->t0 += 50 * ctx->doom->stats.delta;
	if (e->t0 >= 1)
	{
		e->t0 = 0;
		e->cell_x++;
		if (e->cell_y < 5 && e->cell_x == 8)
		{
			e->cell_x = 0;
			e->cell_y++;
		}
		if (e->cell_y == 5 && e->cell_x == 7)
		{
			splice_renderables_array(ctx->doom->renderables, renderables_indexof(ctx->doom->renderables, r), 1);
			return (FALSE);
		}
		set_current_cell(r, e->cell_x, e->cell_y);
	}
	return (TRUE);
}
