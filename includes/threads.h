/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 01:36:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/16 21:09:07 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

# include "render.h"
# include <libft.h>
# include <signal.h>
# include <pthread.h>

# undef _POSIX_C_SOURCE
# define _POSIX_C_SOURCE 199309L

# define RENDER_DATAS_CAPACITY 4096
# define THREADS_COUNT 8


typedef struct			s_thread
{
	pthread_t			pthread;
	t_bool				pending;
	t_vec2				start;
	t_vec2				end;
	t_render_datas		*datas;
	struct s_threads	*threads;
}						t_thread;

typedef struct			s_threads
{
	t_thread			threads[THREADS_COUNT];
	pthread_cond_t		work_cnd;
	pthread_mutex_t		work_mtx;
	t_bool				work;
	pthread_cond_t		wait_cnd;
	pthread_mutex_t		wait_mtx;
	t_bool				wait;
	t_bool				alive;
	int					worker_count;
	int					work_load;
	int					active;
}						t_threads;

void					wake_up_main_thread(t_thread *t);
t_bool					init_threads(t_threads *threads);
void					threads_wait(t_threads *threads);
void					threads_launch(t_threads *threads);
void					threads_clear(t_threads *threads);
t_bool					add_data_to_threads(t_threads *threads,
							t_render_data data);

#endif
