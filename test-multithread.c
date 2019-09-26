#include "libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 3
#define S_HEIGHT 600

typedef struct	s_gdata
{

	pthread_mutex_t	wait_mtx;
	pthread_cond_t	wait_cond;
	int				finished;
}				t_gdata;

typedef struct	s_tdata
{
	int				start;
	int				end;
	t_bool			wait;
	t_gdata			*gdata;
}				t_tdata;

void *worker(void *p)
{
	t_tdata *data = (t_tdata*)p;
	while (1)
	{
		pthread_mutex_lock(&data->gdata->wait_mtx);
		//printf("ENTER\n");
		data->gdata->finished++;
	 	if (data->gdata->finished > THREADS)
		{
			data->wait = TRUE;
			pthread_cond_signal(&data->gdata->wait_cond);
		}
		while (data->wait)
			pthread_cond_wait(&data->gdata->wait_cond, &data->gdata->wait_mtx);
		pthread_mutex_unlock(&data->gdata->wait_mtx);
		printf("start %d end %d wait %d\n", data->start, data->end, data->wait);
	}
}

void main()
{
	pthread_t threads[THREADS];

	t_gdata gdata;
	gdata.finished = THREADS;
	pthread_cond_init(&gdata.wait_cond, NULL);
	pthread_mutex_init(&gdata.wait_mtx, NULL);


	for (int i = 0; i < THREADS; i++)
	{
		t_tdata data;
		int delta = S_HEIGHT / THREADS;
		data.start = delta * i;
		data.end = delta * (i + 1);
		data.gdata = &gdata;
		data.wait = FALSE;
		pthread_create(&threads[i], NULL, worker, &data);
	}

	while (1)
	{
		printf("-----\n");
		fflush(0);
		gdata.finished = 0;
		pthread_cond_signal(&gdata.wait_cond);
		while (gdata.finished <= THREADS)
			continue;
		
		printf("print image\n");
		
	}
}