/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 01:57:57 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/24 02:49:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"

t_threadpool	*threadpool_init(int num_threads)
{
	t_threadpool	*thpool_p;
	int				n;

	threads_on_hold   = 0;
	threads_keepalive = 1;

	if (num_threads < 0)
		num_threads = 0;
	if (!(thpool_p = malloc(sizeof(t_threadpool))))
	{
		threadpool_err("threadpool_init(): Could not allocate memory for thread pool\n");
		return NULL;
	}
	thpool_p->num_threads_alive   = 0;
	thpool_p->num_threads_working = 0;

	if (!jobqueue_init(&thpool_p->jobqueue))
	{
		threadpool_err("threadpool_init(): Could not allocate memory for job queue\n");
		free(thpool_p);
		return NULL;
	}
	if (!(thpool_p->threads = malloc(num_threads * sizeof(t_thread *))))
	{
		threadpool_err("threadpool_init(): Could not allocate memory for threads\n");
		jobqueue_destroy(&thpool_p->jobqueue);
		free(thpool_p);
		return NULL;
	}
	pthread_mutex_init(&(thpool_p->thcount_lock), NULL);
	pthread_cond_init(&thpool_p->threads_all_idle, NULL);

	n = -1;
	while (++n < num_threads)
	{
		if (!thread_init(thpool_p, &thpool_p->threads[n], n))
		{
			free(thpool_p->threads);
			free(thpool_p);
			return (NULL);
		}
	}
	while (thpool_p->num_threads_alive != num_threads)
		;
	return thpool_p;
}

t_bool			threadpool_add_work(t_threadpool *thpool_p, void (*function_p)(void *), void *arg_p)
{
	t_job *newjob;

	if (!(newjob = malloc(sizeof(t_job))))
	{
		threadpool_err("threadpool_add_work(): Could not allocate memory for new job\n");
		return (FALSE);
	}
	newjob->function=function_p;
	newjob->arg=arg_p;
	jobqueue_push(&thpool_p->jobqueue, newjob);
	return (TRUE);
}

void			threadpool_wait(t_threadpool *thpool_p)
{
	pthread_mutex_lock(&thpool_p->thcount_lock);
	while (thpool_p->jobqueue.len || thpool_p->num_threads_working)
		pthread_cond_wait(&thpool_p->threads_all_idle, &thpool_p->thcount_lock);
	pthread_mutex_unlock(&thpool_p->thcount_lock);
}

void			threadpool_destroy(t_threadpool *thpool_p)
{
	volatile int	threads_total;
	double			tpassed;
	time_t			start;
	time_t			end;
	int				n;

	if (thpool_p == NULL)
		return ;
	threads_total = thpool_p->num_threads_alive;
	threads_keepalive = 0;
	tpassed = 0.0;
	time (&start);
	while (tpassed < 1.0 && thpool_p->num_threads_alive)
	{
		bsem_post_all(thpool_p->jobqueue.has_jobs);
		time (&end);
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

void			threadpool_pause(t_threadpool *thpool_p)
{
	int n;

	n = -1;
	while (++n < thpool_p->num_threads_alive)
		pthread_kill(thpool_p->threads[n]->pthread, SIGUSR1);
}