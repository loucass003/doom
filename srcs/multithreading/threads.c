/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 11:16:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/27 12:26:40 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"
#include <stdlib.h>
#include <stdio.h>

void		*worker(t_thread *t)
{
	while (1)
	{
		
		bsem_wait(&t->threads->has_work);
		pthread_mutex_lock(&t->threads->thcount_lock);
		t->threads->working++;
		pthread_mutex_unlock(&t->threads->thcount_lock);
		ft_putstr("JOB\n");
		pthread_mutex_lock(&t->threads->thcount_lock);
		t->threads->working--;
		if (t->threads->working == 0)
			pthread_cond_signal(&t->threads->threads_all_idle);
		pthread_mutex_unlock(&t->threads->thcount_lock);
	}
	return (NULL);
}

t_bool		init_threads(t_threads *threads)
{
	int			i;
	t_thread	*t;


	bsem_init(&threads->has_work, 0);
	threads->working = 0;
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
	printf("WAIT\n");
	bsem_post_all(&threads->has_work);
	pthread_mutex_lock(&threads->thcount_lock);
	while (threads->working)
		pthread_cond_wait(&threads->threads_all_idle, &threads->thcount_lock);
	pthread_mutex_unlock(&threads->thcount_lock);
	bsem_reset(&threads->has_work);
}