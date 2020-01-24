/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 02:28:43 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/24 02:49:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"

t_bool		thread_init(t_threadpool *thpool_p, t_thread **thread_p, int id)
{
	if (!(*thread_p = malloc(sizeof(t_thread))))
	{
		threadpool_err("thread_init(): Could not allocate memory for thread\n");
		return (FALSE);
	}
	(*thread_p)->thpool_p = thpool_p;
	(*thread_p)->id = id;
	pthread_create(&(*thread_p)->pthread, NULL, (void *)thread_do, (*thread_p));
	pthread_detach((*thread_p)->pthread);
	return (TRUE);
}

void		thread_hold(int sig_id)
{
    (void)sig_id;
	threads_on_hold = 1;
	while (threads_on_hold)
		sleep(1);
}

void		*thread_do(t_thread *thread_p)
{
	t_threadpool		*thpool_p;
	struct sigaction	act;
	t_job				*job_p;

	thpool_p = thread_p->thpool_p;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = thread_hold;
	if (sigaction(SIGUSR1, &act, NULL) == -1)
		threadpool_err("thread_do(): cannot handle SIGUSR1");
	pthread_mutex_lock(&thpool_p->thcount_lock);
	thpool_p->num_threads_alive += 1;
	pthread_mutex_unlock(&thpool_p->thcount_lock);
	while(threads_keepalive)
	{
		bsem_wait(thpool_p->jobqueue.has_jobs);
		if (threads_keepalive)
		{
			pthread_mutex_lock(&thpool_p->thcount_lock);
			thpool_p->num_threads_working++;
			pthread_mutex_unlock(&thpool_p->thcount_lock);
			if ((job_p = jobqueue_pull(&thpool_p->jobqueue)))
			{
				job_p->function(job_p->arg);
				free(job_p);
			}
			pthread_mutex_lock(&thpool_p->thcount_lock);
			thpool_p->num_threads_working--;
			if (!thpool_p->num_threads_working) {
				pthread_cond_signal(&thpool_p->threads_all_idle);
			}
			pthread_mutex_unlock(&thpool_p->thcount_lock);
		}
	}
	pthread_mutex_lock(&thpool_p->thcount_lock);
	thpool_p->num_threads_alive--;
	pthread_mutex_unlock(&thpool_p->thcount_lock);
	return NULL;
}

void 		thread_destroy (t_thread *thread_p)
{
	free(thread_p);
}