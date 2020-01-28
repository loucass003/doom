/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 01:36:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/27 16:25:58 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

# include <libft.h>
# include <pthread.h>
# include "render.h"

# define _POSIX_C_SOURCE (199309L)

# define RENDER_DATAS_CAPACITY (4096)
# define THREADS_ROW (3)
# define THREADS_COUNT (16)

typedef struct	s_bsem 
{
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	int						v;
}				t_bsem;

typedef struct	s_thread
{
	pthread_t				pthread;
	t_bool					pending;
	t_vec2					start;
	t_vec2					end;
	t_render_datas			*datas;
	struct s_threads		*threads;
}				t_thread;

typedef struct	s_threads
{
	t_thread		threads[THREADS_COUNT];
	pthread_cond_t	work_cnd;
	pthread_mutex_t	work_mtx;
	t_bool			work;
	pthread_cond_t	wait_cnd;
	pthread_mutex_t	wait_mtx;
	t_bool			wait;
	t_bool			alive;
	int				worker_count;
	int				work_load;
	int				active;
}				t_threads;

void			bsem_init(t_bsem *bsem_p, int value);
void			bsem_reset(t_bsem *bsem_p) ;
void			bsem_post(t_bsem *bsem_p);
void			bsem_post_all(t_bsem *bsem_p);
void			bsem_wait(t_bsem* bsem_p);

t_bool			init_threads(t_threads *threads);
void			threads_wait(t_threads *threads);
void			threads_launch(t_threads *threads);
void			threads_clear(t_threads *threads);
t_bool			add_data_to_threads(t_threads *threads, t_render_data data);

#endif
