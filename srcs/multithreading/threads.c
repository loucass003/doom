/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 11:16:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:14:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "constants.h"
#include "render.h"

void				threads_launch(t_threads *threads)
{
	int i;

	i = -1;
	pthread_mutex_lock(&threads->work_mtx);
	threads->work_load = threads->worker_count;
	while (++i < threads->worker_count)
		threads->threads[i].pending = TRUE;
	pthread_mutex_lock(&threads->wait_mtx);
	threads->wait = TRUE;
	pthread_mutex_unlock(&threads->wait_mtx);
	i = -1;
	while (++i < threads->worker_count)
		pthread_cond_broadcast(&threads->work_cnd);
	pthread_mutex_unlock(&threads->work_mtx);
}

void				*worker(t_thread *t)
{
	int		i;

	while (1)
	{
		pthread_mutex_lock(&t->threads->work_mtx);
		t->threads->active--;
		t->threads->work_load--;
		wake_up_main_thread(t);
		while (!t->pending)
			pthread_cond_wait(&t->threads->work_cnd, &t->threads->work_mtx);
		t->threads->active++;
		t->pending = FALSE;
		pthread_mutex_unlock(&t->threads->work_mtx);
		if (!t->threads->alive)
			break ;
		i = -1;
		while (++i < t->datas->len)
			draw_triangle(t->datas->values[i]);
	}
	return (NULL);
}

static void			init(t_threads *threads)
{
	pthread_cond_init(&threads->wait_cnd, NULL);
	pthread_cond_init(&threads->work_cnd, NULL);
	pthread_mutex_init(&threads->wait_mtx, NULL);
	pthread_mutex_init(&threads->work_mtx, NULL);
	threads->work = FALSE;
	threads->wait = FALSE;
	threads->alive = TRUE;
	threads->work_load = 0;
	threads->worker_count = THREADS_COUNT;
	threads->active = THREADS_COUNT;
}

t_bool				init_threads(t_threads *threads)
{
	int			i;
	t_thread	*t;

	init(threads);
	i = -1;
	while (++i < THREADS_COUNT)
	{
		t = &threads->threads[i];
		t->threads = threads;
		t->pending = FALSE;
		t->start.y = i * (S_HEIGHT / THREADS_COUNT);
		t->start.x = 0;
		t->end.y = (i + 1) * (S_HEIGHT / THREADS_COUNT);
		t->end.x = S_WIDTH;
		if (!(t->datas = create_render_data_array(RENDER_DATAS_CAPACITY)))
			return (FALSE);
		pthread_create(&t->pthread, NULL, (void *)worker, t);
		pthread_detach(t->pthread);
	}
	return (TRUE);
}
