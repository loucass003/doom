/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:41:07 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/26 02:12:14 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"
#include "doom.h"

t_threadpool	*threadpool_init(t_doom *doom, int num_threads, t_threadpool_type type)
{
	t_threadpool	*thpool_p;
	int				n;
	int 			j;

	if (num_threads < 0)
		num_threads = 0;
	if (!(thpool_p = malloc(sizeof(t_threadpool))))
		return (NULL);
	thpool_p->threads_on_hold = 0;
	thpool_p->threads_keepalive = 1;
	thpool_p->num_threads_alive = 0;
	thpool_p->num_threads_working = 0;
	thpool_p->type = type;
	thpool_p->doom = doom;
	if (!jobqueue_init(&thpool_p->jobqueue))
	{
		free(thpool_p);
		return (NULL);
	}
	if (!(thpool_p->threads = malloc(num_threads * sizeof(t_thread *))))
	{
		jobqueue_destroy(&thpool_p->jobqueue);
		free(thpool_p);
		return (NULL);
	}
	pthread_mutex_init(&(thpool_p->thcount_lock), NULL);
	pthread_cond_init(&thpool_p->threads_all_idle, NULL);
	n = -1;
	while (++n < num_threads)
	{
		if (!thread_init(thpool_p, &thpool_p->threads[n], n))
		{
			jobqueue_destroy(&thpool_p->jobqueue);
			j = -1;
			while (++j < num_threads)
				thread_destroy(thpool_p->threads[j]);
			free(thpool_p);
			return (NULL);
		}
	}
	while (thpool_p->num_threads_alive != num_threads)
		;
	return (thpool_p);
}

t_bool		threadpool_add_work(t_threadpool *thpool_p, void (*function_p)(t_thread *t, t_render_data data), t_render_data data)
{
	t_job		newjob;
	
	newjob.function = function_p;
	newjob.data = data;
	return (jobqueue_push(&thpool_p->jobqueue, newjob));
}

void		threadpool_wait(t_threadpool *thpool_p)
{
	pthread_mutex_lock(&thpool_p->thcount_lock);
	while (thpool_p->jobqueue.len || thpool_p->num_threads_working)
		pthread_cond_wait(&thpool_p->threads_all_idle, &thpool_p->thcount_lock);
	pthread_mutex_unlock(&thpool_p->thcount_lock);
}

void		threadpool_destroy(t_threadpool *thpool_p)
{
	int				n;
	volatile int	threads_total;
	time_t			start;
	time_t			end;
	double			tpassed;

	if (thpool_p == NULL)
		return ;
	threads_total = thpool_p->num_threads_alive;
	thpool_p->threads_keepalive = 0;
	tpassed = 0.0;
	time (&start);
	while (tpassed < 1.0 && thpool_p->num_threads_alive)
	{
		bsem_post_all(thpool_p->jobqueue.has_jobs);
		time(&end);
		tpassed = difftime(end,start);
	}
	while (thpool_p->num_threads_alive)
	{
		bsem_post_all(thpool_p->jobqueue.has_jobs);
		sleep(1);
	}
	jobqueue_destroy(&thpool_p->jobqueue);
	n = -1;
	while (++n < threads_total)
		thread_destroy(thpool_p->threads[n]);
	free(thpool_p->threads);
	free(thpool_p);
}


void		threadpool_pause(t_threadpool *thpool_p)
{
	int		i;

	i = -1;
	while (++i < thpool_p->num_threads_alive)
		pthread_kill(thpool_p->threads[i]->pthread, SIGUSR1);
}

void		threadpool_resume(t_threadpool *thpool_p)
{
    (void)thpool_p;
	thpool_p->threads_on_hold = 0;
}

int			threadpool_num_threads_working(t_threadpool *thpool_p)
{
	return thpool_p->num_threads_working;
}