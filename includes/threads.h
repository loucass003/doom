/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 01:36:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/27 12:23:35 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

# include <libft.h>
# include <pthread.h>

# define THREAD_ROW (2)
# define THREADS_COUNT (THREAD_ROW * THREAD_ROW)

typedef struct	s_bsem 
{
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	int						v;
}				t_bsem;

typedef struct	s_thread
{
	pthread_t				pthread;
	struct s_threads		*threads;
}				t_thread;

typedef struct	s_threads
{
	t_thread		threads[THREADS_COUNT];
	t_bsem			has_work;
	int				working;
	pthread_mutex_t	thcount_lock;
	pthread_cond_t	threads_all_idle;
}				t_threads;

void			bsem_init(t_bsem *bsem_p, int value);
void			bsem_reset(t_bsem *bsem_p) ;
void			bsem_post(t_bsem *bsem_p);
void			bsem_post_all(t_bsem *bsem_p);
void			bsem_wait(t_bsem* bsem_p);

t_bool			init_threads(t_threads *threads);
void			threads_wait(t_threads *threads);

#endif
