#include <pthread.h>
#include "libft.h"
#include <stdio.h>

#define THREADS 6
#define S_HEIGHT 600

typedef struct	s_gdata		t_gdata;

typedef struct	s_tdata
{
	int			start_y;
	int			end_y;
	t_bool		pending;
	t_gdata		*gdata;
}				t_tdata;

struct	s_gdata
{
	pthread_mutex_t		work_mutex;
	pthread_cond_t		work_cond;
	pthread_mutex_t		wait_mutex;
	pthread_cond_t		wait_cond;
	t_bool				wait;
	int					working;
	t_tdata				workers[THREADS];
};

void			*worker(void *p)
{
	t_tdata		*data;

	data = (t_tdata *)p;
	while (1)
	{
		pthread_mutex_lock(&data->gdata->work_mutex);
		data->gdata->working--;
		if (data->gdata->working == 0)
		{
			pthread_mutex_lock(&data->gdata->wait_mutex);
			data->gdata->wait = FALSE;
			pthread_cond_broadcast(&data->gdata->wait_cond);
			pthread_mutex_unlock(&data->gdata->wait_mutex);
		}
		while (!data->pending)
			pthread_cond_wait(&data->gdata->work_cond, &data->gdata->work_mutex);
		data->pending = FALSE;
		pthread_mutex_unlock(&data->gdata->work_mutex);
		printf("%d %d\n", data->start_y, data->end_y);
	}
	return (NULL);
}

int				main(void)
{
	pthread_t	threads[THREADS];
//	t_tdata		datas[THREADS];


	t_gdata		gdata;
	ft_bzero(&gdata, sizeof(t_gdata));
	int	delta = S_HEIGHT / THREADS;
	for (int i = 0; i < THREADS; i++)
	{
		t_tdata	*data = &gdata.workers[i];

		data->start_y = delta * i;
		data->end_y = delta * (i + 1);
		data->pending = FALSE;
		data->gdata = &gdata;
		pthread_create(&threads[i], NULL, worker, data);
	}

	while (1)
	{
		printf("******\n");
		pthread_mutex_lock(&gdata.work_mutex);
		gdata.working = THREADS;
		for (int i = 0; i < THREADS; i++)
			gdata.workers[i].pending = TRUE;
		pthread_mutex_lock(&gdata.wait_mutex);
		gdata.wait = TRUE;
		while (gdata.wait)
			pthread_cond_wait(&gdata.wait_cond, &gdata.wait_mutex);
		pthread_mutex_unlock(&gdata.wait_mutex);
		for (int i = 0; i < THREADS; i++)
			pthread_cond_broadcast(&gdata.work_cond);
		pthread_mutex_unlock(&gdata.work_mutex);
		printf("PROCESS FRAME\n");
	}
	return (0);
}
