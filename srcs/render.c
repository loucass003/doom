/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/18 17:49:59 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include "sprite.h"
#include "render.h"
#include "octree.h"
#include "door.h"
#include "explosion.h"

void	render(t_doom *doom)
{
	t_gui	*gui;

	if (doom->current_gui < 0)
		return ;
	gui = &doom->guis[doom->current_gui];
	gui->render(gui, doom);
}

t_bool	update_renderable(t_render_context *ctx, t_renderable *r)
{
	if (r->of.type == RENDERABLE_ENTITY)
		return (update_renderable_entity(ctx, r));
	else if (r->of.type == RENDERABLE_DOOR)
		return (update_renderable_door(ctx, r));
	else if (r->of.type == RENDERABLE_EXPLOSION)
		return (update_renderable_explosion(ctx, r));
	return (TRUE);
}

t_bool	pre_render(t_render_context *ctx, t_renderable *r)
{
	int	i;

	if (!update_renderable(ctx, r))
		return (FALSE);
	if (r->dirty || (r->sprite && r->sprite->always_facing_player))
	{
		if (r->sprite && r->sprite->always_facing_player)
			r->rotation.y = ctx->camera->rotation.y;
		transform_renderable(r);
		r->dirty = FALSE;
	}
	if (!r->visible)
		return (TRUE);
	i = -1;
	while (++i < r->faces->len)
		r->faces->values[i].rendered = FALSE;
	return (TRUE);
}

t_bool	render_renderable(t_render_context *ctx, t_renderable *r)
{
	int			i;
	t_face_data	face_data;

	if (!pre_render(ctx, r))
		return (FALSE);
	face_data = (t_face_data){ .ctx = ctx, .r = r };
	if (r->octree)
	{
		frustum_to_local(ctx->camera, r);
		frustum_intersect_octree(r->octree, ctx->camera->frustum,
			render_face, &face_data);
	}
	else
	{
		i = -1;
		while (++i < r->faces->len)
			render_face(i, &face_data);
	}
	return (TRUE);
}
