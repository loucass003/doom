/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool_job.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 02:15:03 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/26 03:34:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threadpool.h"

t_bool		jobqueue_init(t_jobqueue *jobqueue_p)
{
	const int capacity = 4096;

	if (!(jobqueue_p->values = ft_memalloc(sizeof(t_job) * capacity)))
		return (FALSE);
	jobqueue_p->len = 0;
	jobqueue_p->capacity = capacity;
	if (!(jobqueue_p->has_jobs = malloc(sizeof(t_bsem))))
		return (FALSE);
	pthread_mutex_init(&(jobqueue_p->rwmutex), NULL);
	bsem_init(jobqueue_p->has_jobs, 0);
	return (TRUE);
}

void		jobqueue_clear(t_jobqueue *jobqueue_p)
{
	printf("CLEAR\n");
	bsem_reset(jobqueue_p->has_jobs);
	jobqueue_p->len = 0;
}

t_bool		jobqueue_push(t_jobqueue *jobqueue_p, t_job newjob)
{
	t_job	*new;

	pthread_mutex_lock(&jobqueue_p->rwmutex);
	if (jobqueue_p->len == jobqueue_p->capacity)
	{
		jobqueue_p->capacity *= 2;
		if (!(new = ft_memalloc(sizeof(t_job) * jobqueue_p->capacity)))
			return (FALSE);
		ft_memcpy(new, jobqueue_p->values, sizeof(t_job) * jobqueue_p->len);
		free(jobqueue_p->values);
		jobqueue_p->values = new;
	}
	jobqueue_p->values[jobqueue_p->len] = newjob;
	jobqueue_p->len++;
	
	pthread_mutex_unlock(&jobqueue_p->rwmutex);
	return (TRUE);
}

t_job		jobqueue_pull(t_jobqueue *jobqueue_p, t_bool *nodata)
{
	t_job	job_p;

	*nodata = FALSE;

	job_p = (t_job){ .function = NULL };
	pthread_mutex_lock(&jobqueue_p->rwmutex);
	if (jobqueue_p->len == 0)
		*nodata = TRUE;
	else if (jobqueue_p->len == 1)
	{
		job_p = jobqueue_p->values[0];
		jobqueue_p->len = 0;
	}
	else
	{
		job_p = jobqueue_p->values[jobqueue_p->len - 1];
		jobqueue_p->len--;
		bsem_post(jobqueue_p->has_jobs);
	}
	pthread_mutex_unlock(&jobqueue_p->rwmutex);
	return (job_p);
}

void		jobqueue_destroy(t_jobqueue *jobqueue_p)
{
	jobqueue_clear(jobqueue_p);
	free(jobqueue_p->values);
	free(jobqueue_p->has_jobs);
}
