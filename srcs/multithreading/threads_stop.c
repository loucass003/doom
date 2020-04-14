/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_stop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:09:51 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:13:37 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "threads.h"

static void			threads_stop_threads(t_threads *threads)
{
	int		i;

	threads->alive = FALSE;
	threads_launch(threads);
	i = -1;
	while (++i < threads->worker_count)
		pthread_join(threads->threads[i].pthread, NULL);
}

void				threads_destroy(t_threads *threads)
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
