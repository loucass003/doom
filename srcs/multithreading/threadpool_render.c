/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/26 00:33:36 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/26 03:34:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"
#include "constants.h"

t_bool		threadpool_render_reset(t_threadpool *thpool_p, t_render_context *ctx)
{
	int			i;
	t_thread	*thread;

	if (thpool_p->type != THPOOL_RENDER)
		return (FALSE);
	i = -1;
	while (++i < thpool_p->num_threads_alive)
	{
		thread = thpool_p->threads[i];
		thread->ctx.type = ctx->type;
		clear_image(thread->ctx.image);
		ft_bzero(thread->ctx.buffer, sizeof(float) * (S_WIDTH * S_HEIGHT));
		// for (int j = 0; j < S_WIDTH * S_HEIGHT; j++)
		// 	thread->ctx.buffer[j] = 0;
	}
	return (TRUE);
}

t_bool		threadpool_render_merge(t_threadpool *thpool_p, t_render_context *ctx)
{
	int			i;
	int			j;
	t_thread	*thread;

	if (thpool_p->type != THPOOL_RENDER)
		return (FALSE);
	i = -1;
	while (++i < thpool_p->num_threads_alive)
	{
		thread = thpool_p->threads[i];
		j = -1;
		while (++j < S_WIDTH * S_HEIGHT)
			if (ctx->buffer[j] > thread->ctx.buffer[j])
			{
				ctx->buffer[j] = thread->ctx.buffer[j];
				ctx->image->pixels[j] = thread->ctx.image->pixels[j];
			}
	}
	return (TRUE);
}