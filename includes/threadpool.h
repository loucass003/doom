/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threadpool.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 01:36:30 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/24 17:52:32 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADPOOL_H
# define THREADPOOL_H

# include <libft.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <errno.h>
# include <time.h>
# include "render.h"

typedef enum	s_threadpool_type
{
	THPOOL_SIMPLE,
	THPOOL_RENDER
}				t_threadpool_type;

typedef struct	s_bsem 
{
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	int						v;
}				t_bsem;

typedef struct	s_thread
{
	int						id;
	pthread_t				pthread;
	t_render_context		ctx;
	struct s_threadpool*	thpool_p;
}				t_thread;

typedef struct	s_job
{
	struct s_job*			prev;
	void					(*function)(t_thread *thread, void* arg);
	void*					arg;
}				t_job;

typedef struct	s_jobqueue
{
	pthread_mutex_t			rwmutex;
	t_job					*front;
	t_job					*rear;
	t_bsem					*has_jobs;
	int						len;
}				t_jobqueue;

typedef struct	s_threadpool
{
	t_threadpool_type		type;
	t_thread**				threads;
	volatile int			threads_keepalive;
	volatile int			threads_on_hold;
	volatile int			num_threads_alive;
	volatile int			num_threads_working;
	pthread_mutex_t			thcount_lock;
	pthread_cond_t			threads_all_idle;
	t_jobqueue				jobqueue;
	struct s_doom			*doom;
}				t_threadpool;

t_bool			thread_init(t_threadpool *thpool_p, t_thread **thread_p, int id);
void			thread_hold(int sig_id);
void			*thread_do(t_thread *thread_p);
void			thread_destroy (t_thread *thread_p);

t_bool			jobqueue_init(t_jobqueue *jobqueue_p);
void			jobqueue_clear(t_jobqueue *jobqueue_p);
void			jobqueue_push(t_jobqueue *jobqueue_p, t_job *newjob);
t_job			*jobqueue_pull(t_jobqueue *jobqueue_p);
void			jobqueue_destroy(t_jobqueue *jobqueue_p);

void			bsem_init(t_bsem *bsem_p, int value);
void			bsem_reset(t_bsem *bsem_p) ;
void			bsem_post(t_bsem *bsem_p);
void			bsem_post_all(t_bsem *bsem_p);
void			bsem_wait(t_bsem* bsem_p);

void			threadpool_err(char *str);

t_threadpool	*threadpool_init(struct s_doom *doom, int num_threads, t_threadpool_type type);
t_bool			threadpool_add_work(t_threadpool *thpool_p, void (*function_p)(t_thread *t, void *), void *arg_p);
void			threadpool_wait(t_threadpool *thpool_p);
void			threadpool_destroy(t_threadpool *thpool_p);
void			threadpool_pause(t_threadpool *thpool_p);

#endif
