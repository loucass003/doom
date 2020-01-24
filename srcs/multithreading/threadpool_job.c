/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool_job.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:30:00 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/24 15:33:17 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"

t_bool		jobqueue_init(t_jobqueue *jobqueue_p)
{
	jobqueue_p->len = 0;
	jobqueue_p->front = NULL;
	jobqueue_p->rear  = NULL;
	jobqueue_p->has_jobs = malloc(sizeof(t_bsem));
	if (jobqueue_p->has_jobs == NULL)
		return (FALSE);
	pthread_mutex_init(&(jobqueue_p->rwmutex), NULL);
	bsem_init(jobqueue_p->has_jobs, 0);
	return (TRUE);
}

void		jobqueue_clear(t_jobqueue *jobqueue_p)
{
	while(jobqueue_p->len)
		free(jobqueue_pull(jobqueue_p));
	jobqueue_p->front = NULL;
	jobqueue_p->rear  = NULL;
	bsem_reset(jobqueue_p->has_jobs);
	jobqueue_p->len = 0;
}

void		jobqueue_push(t_jobqueue *jobqueue_p, t_job *newjob)
{
	pthread_mutex_lock(&jobqueue_p->rwmutex);
	newjob->prev = NULL;
	if (jobqueue_p->len == 0)
	{
		jobqueue_p->front = newjob;
		jobqueue_p->rear  = newjob;
	}
	else
	{
		jobqueue_p->rear->prev = newjob;
		jobqueue_p->rear = newjob;
	}
	jobqueue_p->len++;
	bsem_post(jobqueue_p->has_jobs);
	pthread_mutex_unlock(&jobqueue_p->rwmutex);
}

t_job		*jobqueue_pull(t_jobqueue *jobqueue_p)
{
	t_job	*job_p;

	pthread_mutex_lock(&jobqueue_p->rwmutex);
	job_p = jobqueue_p->front;
	if (jobqueue_p->len == 0)
		;
	else if (jobqueue_p->len == 1)
	{
		jobqueue_p->front = NULL;
		jobqueue_p->rear  = NULL;
		jobqueue_p->len = 0;
	}
	else
	{
		jobqueue_p->front = job_p->prev;
		jobqueue_p->len--;
		bsem_post(jobqueue_p->has_jobs);
	}
	pthread_mutex_unlock(&jobqueue_p->rwmutex);
	return (job_p);
}

void		jobqueue_destroy(t_jobqueue *jobqueue_p)
{
	jobqueue_clear(jobqueue_p);
	free(jobqueue_p->has_jobs);
}
