/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gaps_filler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 13:36:42 by llelievr          #+#    #+#             */
/*   Updated: 2019/12/09 19:15:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include "arrays.h"

typedef struct	s_gaps
{
	t_vec2			bounds;
	t_2dvertices	*ranges;
}				t_gaps;

void		ft_swap2(t_vec2 *a, t_vec2 *b)
{
	t_vec2 t;

	t = *a;
	*a = *b;
	*b = t;
}

void		sort_ranges(t_2dvertices *range)
{
	int		i;
	int		j;
	t_vec2	*v0;
	t_vec2	*v1;

	i = -1;
	while (++i < range->len - 1)
	{
		j = -1;
		while (++j < range->len - i - 1)
		{
			v0 = &range->vertices[j];
			v1 = &range->vertices[j + 1];
			if (v1->x < v0->x)
				ft_swap2(v0, v1);
		}
	}
}

void		test_gaps(void)
{
	t_gaps	gaps;

	gaps.bounds = (t_vec2){ 0, 15 };
	gaps.ranges = create_2dvertices_array(5);
	append_2dvertices_array(&gaps.ranges, (t_vec2){ -5, 20 });
	sort_ranges(gaps.ranges);
	float start = gaps.bounds.x;
	for (int i = 0; i < gaps.ranges->len; i++)
	{
		t_vec2	r = gaps.ranges->vertices[i];
		if (start < r.x)
		{
			printf("gap (%f %f)\n", start, r.x);
			start = r.y;
		}
		else if (r.x >= gaps.bounds.x)
			start = r.x;
	}
	if (start < gaps.bounds.y && start != gaps.bounds.x)
		printf("gap (%f %f)\n", start, gaps.bounds.y);
}
