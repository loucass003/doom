/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:10:53 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/15 17:13:53 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "threads.h"

void		wake_up_main_thread(t_thread *t)
{
	if (t->threads->work_load == 0)
	{
		pthread_mutex_lock(&t->threads->wait_mtx);
		t->threads->wait = FALSE;
		pthread_cond_broadcast(&t->threads->wait_cnd);
		pthread_mutex_unlock(&t->threads->wait_mtx);
	}
}

void		threads_clear(t_threads *threads)
{
	int		i;

	i = -1;
	while (++i < threads->worker_count)
		threads->threads[i].datas->len = 0;
}

void		threads_wait(t_threads *threads)
{
	pthread_mutex_lock(&threads->wait_mtx);
	while (threads->wait)
		pthread_cond_wait(&threads->wait_cnd, &threads->wait_mtx);
	pthread_mutex_unlock(&threads->wait_mtx);
}
