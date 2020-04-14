/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:08:32 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/14 20:09:03 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "threads.h"
#include <libft.h>
#include "render.h"

static inline void	swap(t_vertex *a, t_vertex *b)
{
	t_vertex t;

	t = *a;
	*a = *b;
	*b = t;
}

t_bool				add_data_to_threads(t_threads *threads, t_render_data data)
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
		if (data.triangle.a.pos.y > t->end.y
			|| data.triangle.c.pos.y < t->start.y)
			continue;
		data.min = t->start;
		data.max = t->end;
		if (!append_render_data_array(&t->datas, data))
			return (FALSE);
	}
	return (TRUE);
}
