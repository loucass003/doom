/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 03:01:33 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 22:29:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include "doom.h"
#include "door.h"

static t_bool			is_something_close(t_render_context *ctx,
	t_renderable *r)
{
	int				i;
	t_doom			*doom;
	t_renderable	renderable;

	i = -1;
	doom = ctx->doom;
	while (++i < doom->renderables->len)
	{
		renderable = doom->renderables->values[i];
		if (renderable.of.type != RENDERABLE_ENTITY
			|| renderable.of.data.entity->dead)
			continue;
		if (ft_vec3_len(ft_vec3_sub(r->position,
			renderable.of.data.entity->position)) < 12)
			return (TRUE);
	}
	return (FALSE);
}

void					update_door_open(t_render_context *ctx, t_renderable *r,
	t_door *door)
{
	float		open_speed;

	open_speed = 0.8 * ctx->doom->stats.delta;
	if (is_something_close(ctx, r))
	{
		if (door->open_value < 1)
			door->open_value += open_speed;
		if (door->open_value > 1)
			door->open_value = 1;
	}
	else
	{
		if (door->open_value > 0)
			door->open_value -= open_speed;
		if (door->open_value < 0)
			door->open_value = 0;
	}
}

t_bool					update_renderable_door(t_render_context *ctx,
	t_renderable *r)
{
	t_door		*door;
	int			i;
	float		last_open;
	t_face		*face;

	door = r->of.data.door;
	if (ctx->type != CTX_NORMAL)
		return (TRUE);
	last_open = door->open_value;
	update_door_open(ctx, r, door);
	if (last_open != door->open_value)
	{
		if (last_open == 0 || last_open == 1)
			play_music(&ctx->doom->audio, r->position, 9, FALSE);
		i = -1;
		while (++i < r->faces->len)
		{
			face = &r->faces->values[i];
			if (face->group == door->door_1 || face->group == door->door_2)
				face->has_collision = door->open_value == 0;
		}
		r->dirty = TRUE;
	}
	return (TRUE);
}
