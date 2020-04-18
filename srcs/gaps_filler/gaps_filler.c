/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gaps_filler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 13:36:42 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/19 00:51:51 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "arrays.h"
#include "editor.h"
#include "doom.h"

static void		swap(t_vec4 *a, t_vec4 *b)
{
	t_vec4 t;

	t = *a;
	*a = *b;
	*b = t;
}

void			sort_ranges(t_4dvertices *range)
{
	int		i;
	int		j;
	t_vec4	*v0;
	t_vec4	*v1;

	i = -1;
	while (++i < range->len - 1)
	{
		j = -1;
		while (++j < range->len - i - 1)
		{
			v0 = &range->vertices[j];
			v1 = &range->vertices[j + 1];
			if (v1->x < v0->x)
				swap(v0, v1);
		}
	}
}
