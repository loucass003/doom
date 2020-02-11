/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 11:16:46 by llelievr          #+#    #+#             */
/*   Updated: 2020/02/11 09:26:16 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "constants.h"
#include "render.h"

static	void		wake_up_main_thread(t_thread *t)
{
	if (t->threads->work_load == 0)
	{
		pthread_mutex_lock(&t->threads->wait_mtx);
		t->threads->wait = FALSE;
		pthread_cond_broadcast(&t->threads->wait_cnd);
		pthread_mutex_unlock(&t->threads->wait_mtx);
	}
}

void				threads_clear(t_threads *threads)
{
	int		i;

	i = -1;
	while (++i < threads->worker_count)
		threads->threads[i].datas->len = 0;
}

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

static inline void	swap(t_vertex *a, t_vertex *b)
{
	t_vertex t = *a;
	*a = *b;
	*b = t;
}

t_bool		add_data_to_threads(t_threads *threads, t_render_data data)
{
	int			i;
	t_thread	*t;

	if (data.triangle.b.pos.y < data.triangle.a.pos.y)
		swap(&data.triangle.a, &data.triangle.b);
	if (data.triangle.c.pos.y < data.triangle.a.pos.y)
		swap(&data.triangle.a, &data.triangle.c);
	if (data.triangle.c.pos.y < data.triangle.b.pos.y)
		swap(&data.triangle.b, &data.triangle.c);
	i = -1;
	while (++i < threads->worker_count)
	{
		t = &threads->threads[i];
		
		if (data.triangle.a.pos.y > t->end.y || data.triangle.c.pos.y < t->start.y)
			continue;
		data.min = t->start;
		data.max = t->end;
		if (!append_render_data_array(&t->datas, data))
			return (FALSE);
	}
	return (TRUE);
}

void		*worker(t_thread *t)
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

t_bool		init_threads(t_threads *threads)
{
	int			i;
	t_thread	*t;

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

void		threads_wait(t_threads *threads)
{
	pthread_mutex_lock(&threads->wait_mtx);
	while (threads->wait)
		pthread_cond_wait(&threads->wait_cnd, &threads->wait_mtx);
	pthread_mutex_unlock(&threads->wait_mtx);
}

static void	threads_stop_threads(t_threads *threads)
{
	int		i;

	threads->alive = FALSE;
	threads_launch(threads);
	i = -1;
	while (++i < threads->worker_count)
		pthread_join(threads->threads[i].pthread, NULL);
}

void		threads_destroy(t_threads *threads)
{
	int		i;

	if (!threads->worker_count || threads->alive == FALSE)
		return ;
	threads_stop_threads(threads);
	pthread_mutex_destroy(&threads->wait_mtx);
	pthread_mutex_destroy(&threads->work_mtx);
	pthread_cond_destroy(&threads->wait_cnd);
	pthread_cond_destroy(&threads->work_cnd);
	i = -1;
	while (++i < THREADS_COUNT)
		ft_memdel((void **)&threads->threads[i].datas);
}
