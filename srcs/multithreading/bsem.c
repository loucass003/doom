/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsem.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 15:28:55 by llelievr          #+#    #+#             */
/*   Updated: 2020/01/27 11:37:49 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"

void		bsem_init(t_bsem *bsem_p, int value)
{
	if (value < 0 || value > 1) 
		exit(1);
	pthread_mutex_init(&(bsem_p->mutex), NULL);
	pthread_cond_init(&(bsem_p->cond), NULL);
	bsem_p->v = value;
}

void		bsem_reset(t_bsem *bsem_p) 
{
	bsem_init(bsem_p, 0);
}

void		bsem_post(t_bsem *bsem_p) 
{
	pthread_mutex_lock(&bsem_p->mutex);
	bsem_p->v = 1;
	pthread_cond_signal(&bsem_p->cond);
	pthread_mutex_unlock(&bsem_p->mutex);
}

void		bsem_post_all(t_bsem *bsem_p)
{
	pthread_mutex_lock(&bsem_p->mutex);
	bsem_p->v = 1;
	pthread_cond_broadcast(&bsem_p->cond);
	pthread_mutex_unlock(&bsem_p->mutex);
}

void		bsem_wait(t_bsem* bsem_p)
{
	pthread_mutex_lock(&bsem_p->mutex);
	while (bsem_p->v != 1)
		pthread_cond_wait(&bsem_p->cond, &bsem_p->mutex);
	bsem_p->v = 0;
	pthread_mutex_unlock(&bsem_p->mutex);
}
