/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 02:28:43 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/26 03:28:38 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"
#include "doom.h"

t_bool		init_render_context(t_threadpool *thpool_p, t_render_context *ctx)
{
	if (!(ctx->image = malloc(sizeof(t_img))))
		return (FALSE);
	ctx->image->ignore_texture = TRUE;
	if (!create_image(thpool_p->doom->renderer, S_WIDTH, S_HEIGHT, ctx->image))
		return (FALSE);
	if ((ctx->buffer = ft_memalloc(sizeof(float) * (S_WIDTH * S_HEIGHT))))
		return (FALSE);
	return (TRUE);
}

t_bool		thread_init(t_threadpool *thpool_p, t_thread** thread_p, int id)
{
	if (!(*thread_p = malloc(sizeof(t_thread))))
		return (FALSE);
	(*thread_p)->thpool_p = thpool_p;
	(*thread_p)->id = id;
	if (thpool_p->type == THPOOL_RENDER)
		init_render_context(thpool_p, &(*thread_p)->ctx);
	pthread_create(&(*thread_p)->pthread, NULL, (void *)thread_do, (*thread_p));
	pthread_detach((*thread_p)->pthread);
	return (TRUE);
}

void		*thread_do(t_thread *thread_p)
{
	t_job				job_p;
	t_threadpool		*thpool_p;

	thpool_p = thread_p->thpool_p;
	pthread_mutex_lock(&thpool_p->thcount_lock);
	thpool_p->num_threads_alive += 1;
	pthread_mutex_unlock(&thpool_p->thcount_lock);
	while(thpool_p->threads_keepalive)
	{
		bsem_wait(thpool_p->jobqueue.has_jobs);
		if (thpool_p->threads_keepalive)
		{
			pthread_mutex_lock(&thpool_p->thcount_lock);
			thpool_p->num_threads_working++;
			pthread_mutex_unlock(&thpool_p->thcount_lock);
			t_bool nodata = FALSE;
			job_p = jobqueue_pull(&thpool_p->jobqueue, &nodata);
			if (!nodata)
			{
				job_p.function(thread_p, job_p.data);
				//free(job_p);
			}
			pthread_mutex_lock(&thpool_p->thcount_lock);
			thpool_p->num_threads_working--;
			if (!thpool_p->num_threads_working)
				pthread_cond_signal(&thpool_p->threads_all_idle);
			pthread_mutex_unlock(&thpool_p->thcount_lock);
		}
	}
	pthread_mutex_lock(&thpool_p->thcount_lock);
	thpool_p->num_threads_alive--;
	pthread_mutex_unlock(&thpool_p->thcount_lock);
	return NULL;
}

void		thread_destroy(t_thread *thread_p)
{
	free(thread_p);
}
