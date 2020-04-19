/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/19 19:32:06 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 19:34:54 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

float			area(t_4dvertices *vertices, int *filters, int filters_count)
{
	float	a;
	int		p;
	int		q;
	t_vec4	pval;
	t_vec4	qval;

	a = 0.0f;
	p = filters_count - 1;
	q = 0;
	while (q < filters_count)
	{
		pval = vertices->vertices[filters[p]];
		qval = vertices->vertices[filters[q]];
		a += pval.x * qval.y - qval.x * pval.y;
		p = q++;
	}
	return (a * 0.5f);
}

t_bool			inside_triangle(t_vec4 a, t_vec4 b, t_vec4 c, t_vec4 p)
{
	float	i;
	float	j;
	float	k;

	i = (c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x);
	j = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
	k = (a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x);
	return (i >= 0 && j >= 0 && k >= 0);
}

t_bool			free_ret(void *value, t_bool ret)
{
	free(value);
	return (ret);
}
