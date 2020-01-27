/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 11:16:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/27 11:45:06 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"
#include <stdlib.h>
#include <stdio.h>

void		*worker(t_thread *t)
{
	while (1)
	{
		bsem_wait(t->threads->has_jobs);
		ft_putstr("JOB\n");
		pthread_mutex_lock(&t->threads->thcount_lock);
		t->threads->working--;
		if (!t->threads->working)
			pthread_cond_signal(&t->threads->threads_all_idle);
		pthread_mutex_unlock(&t->threads->thcount_lock);
	}
	return (NULL);
}

t_bool		init_threads(t_threads *threads)
{
	int			i;
	t_thread	*t;

	if (!(threads->has_jobs = malloc(sizeof(t_bsem))))
		return (FALSE);
	bsem_init(threads->has_jobs, 0);
	pthread_mutex_init(&threads->thcount_lock, NULL);
	pthread_cond_init(&threads->threads_all_idle, NULL);
	i = -1;
	while (++i < THREADS_COUNT)
	{
		t = &threads->threads[i];
		t->threads = threads;
		pthread_create(&t->pthread, NULL, (void *)worker, t);
		pthread_detach(t->pthread);
	}
	return (TRUE);
}

void		threads_wait(t_threads *threads)
{
	pthread_mutex_lock(&threads->thcount_lock);
	while (threads->working != 0)
		pthread_cond_wait(&threads->threads_all_idle, &threads->thcount_lock);
	pthread_mutex_unlock(&threads->thcount_lock);
	bsem_reset(threads->has_jobs);
}