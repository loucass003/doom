/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools_poly.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:33:38 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/20 16:00:21 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	print_lst(t_poly *poly)
{
	t_poly		*tmp;
	t_line_list *cmp;
	int x;

	x = 1;
	tmp = poly;
	while (tmp != NULL)
	{
		printf("poly %d\n", x);
		x++;
		cmp = tmp->list;
		while (cmp != NULL)
		{
			printf("x = %f - y = %f  ||  ", cmp->line.a.x, cmp->line.a.y);
			printf("x = %f - y = %f\n", cmp->line.b.x, cmp->line.b.y);
			cmp = cmp->next;
		}
		printf("\n");
		tmp = tmp->next;
	}
	printf("\n\n");
}

void	print_poly(t_doom *doom, t_poly *poly)
{
	t_poly		*tmp1;
	t_line_list *tmp;
	int			i;

	tmp1 = poly;
	while (tmp1)
	{
		tmp = tmp1->list;
		while (tmp != NULL)
		{
			draw_line(&doom->screen, (t_pixel){tmp->line.a.x * 20 + 10
				, tmp->line.a.y * 20 + 10, 0xFFFFFF}, (t_pixel)
				{tmp->line.b.x * 20 + 10, tmp->line.b.y * 20 + 10});
			i = -6;
			while (doom->editor.sup != 0 && i++ <= 5)
			{
				doom->screen.pixels[(((int)tmp->line.a.y * 20 + 10) + i)
					* doom->screen.width
					+ ((int)tmp->line.a.x * 20) + 10] = 0xFF0000;
				doom->screen.pixels[((int)tmp->line.a.y * 20 + 10)
					* doom->screen.width
					+ ((int)tmp->line.a.x * 20) + 10 + i] = 0xFF0000;
				doom->screen.pixels[(((int)tmp->line.b.y * 20 + 10) + i)
					* doom->screen.width
					+ ((int)tmp->line.b.x * 20) + 10] = 0xFF0000;
				doom->screen.pixels[((int)tmp->line.b.y * 20 + 10)
					* doom->screen.width
					+ ((int)tmp->line.b.x * 20) + 10 + i] = 0xFF0000;
			}
			tmp = tmp->next;
		}
		tmp1 = tmp1->next;
	}
}

void	check_poly_close(t_doom *doom, t_poly *poly)
{
	t_line_list *cmp;
	t_poly		*tmp;
	int			i;

	tmp = poly;
	while (tmp)
	{
		i = 0;
		cmp = tmp->list;
		while (cmp)
		{
			if ((cmp->line.b.x == doom->editor.first[0]
			&& cmp->line.b.y == doom->editor.first[1])
			|| (cmp->line.b.x == doom->editor.first[0]
			&& cmp->line.b.y == doom->editor.first[1]))
				i++;
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	if (i > 0)
	{
		doom->editor.alert[0] = 1;
		doom->editor.click = -1;
	}
}

t_bool	check_multi_line(t_doom *doom, t_poly *poly, int x1, int y1, int x2, int y2)
{
	t_poly		*tmp;
	t_line_list *cmp;

	tmp = poly;
	while (tmp)
	{
		cmp = tmp->list;
		while (cmp)
		{
			if ((x1 == cmp->line.a.x && y1 == cmp->line.a.y
			&& x2 == cmp->line.b.x && y2 == cmp->line.b.y)
			|| (x1 == cmp->line.b.x && y1 == cmp->line.b.y
			&& x2 == cmp->line.a.x && y2 == cmp->line.a.y))
			{
				doom->editor.alert[2] = 1;
				return (FALSE);
			}
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	return (TRUE);
}

t_bool	check_same_point(t_doom *doom)
{
	if (doom->editor.line.b.x == doom->editor.line.a.x
	&& doom->editor.line.a.y == doom->editor.line.b.y)
	{
		doom->editor.alert[1] = 1;
		return (FALSE);
	}
	return (TRUE);
}

t_bool	check_multi_point(t_doom *doom, t_poly *poly, int x, int y)
{
	t_poly		*tmp;
	t_line_list *cmp;
	int			i;

	tmp = poly;
	while (tmp)
	{
		cmp = tmp->list;
		i = 0;
		while (cmp)
		{
			if ((x == cmp->line.b.x && y == cmp->line.b.y)
			|| (x == cmp->line.a.x && y == cmp->line.a.y))
				i++;
			if (i > 1)
			{
				doom->editor.alert[1] = 1;
				return (FALSE);
			}
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	return (TRUE);
}

t_bool	intersect0(t_line *line, t_line *cmp, t_line_list *pol)
{
	t_line_list *tmp;
	float		a1;
	float		a2;
	float		b1;
	float		b2;
	float		x;
	float		y;

	tmp = pol;
	if (line->b.x == line->a.x || cmp->b.x == cmp->a.x)
	{
		if (line->b.x == line->a.x)
		{
			a2 = (cmp->b.y - cmp->a.y) / (cmp->b.x - cmp->a.x);
			b2 = cmp->a.y - (a2 * cmp->a.x);
			x = line->b.x;
			y = a2 * x + b2;
		}
		else if (cmp->b.x == cmp->a.x)
		{
			a1 = (line->b.y - line->a.y) / (line->b.x - line->a.x);
			b1 = line->a.y - (a1 * line->a.x);
			x = cmp->b.x;
			y = a1 * x + b1;
		}
	}
	else
	{
		a1 = (line->b.y - line->a.y) / (line->b.x - line->a.x);
		a2 = (cmp->b.y - cmp->a.y) / (cmp->b.x - cmp->a.x);
		b1 = line->a.y - (a1 * line->a.x);
		b2 = cmp->a.y - (a2 * cmp->a.x);
		if (a2 == 0 && line->b.x == line->a.x)
		{
			x = line->b.x;
			y = cmp->b.y;
		}
		else if (a1 == 0 && cmp->b.x == cmp->a.x)
		{
			x = cmp->b.x;
			y = line->b.y;
		}
		else if (a2 == 0)
		{
			y = cmp->a.y;
			x = (y - b1) / a1;
		}
		else if (a1 == 0)
		{
			y = line->a.y;
			x = (y - b2) / a2;
		}
		else
		{
			x = (b2 - b1) / (a1 - a2);
			y = a1 * x + b1;
		}
	}
	if (x <= fmax(line->a.x, line->b.x) && x >= fmin(line->a.x, line->b.x)
	&& x <= fmax(cmp->a.x, cmp->b.x) && x >= fmin(cmp->a.x, cmp->b.x)
	&& y <= fmax(line->a.y, line->b.y) && y >= fmin(line->a.y, line->b.y)
	&& y <= fmax(cmp->a.y, cmp->b.y) && y >= fmin(cmp->a.y, cmp->b.y))
	{
		while (tmp)
		{
			if ((tmp->line.a.x == x && tmp->line.a.y == y
			&& x == line->a.x && y == line->a.y)
			|| (tmp->line.a.x == x && tmp->line.a.y == y
			&& x == line->b.x && y == line->b.y)
			|| (tmp->line.b.x == x && tmp->line.b.y == y
			&& x == line->b.x && y == line->b.y)
			|| (tmp->line.b.x == x && tmp->line.b.y == y
			&& x == line->a.x && y == line->a.y))
				return (TRUE);
			tmp = tmp->next;
		}
		return (FALSE);
	}
	return (TRUE);
}

t_bool	check_secant_line(t_doom *doom, t_poly *poly, float x1, float x2, float y1, float y2)
{
	t_poly		*tmp;
	t_vec2		*inter;
	t_line_list *cmp;
	t_line		line;

	line.a.x = x1;
	line.b.x = x2;
	line.a.y = y1;
	line.b.y = y2;
	tmp = poly;
	while (tmp)
	{
		cmp = tmp->list;
		while (cmp)
		{
			if (line.a.x == cmp->line.a.x && line.a.y == cmp->line.a.y
			&& line.b.x == cmp->line.b.x && line.b.y == cmp->line.b.y)
				cmp = cmp->next;
			if (intersect0(&line, &cmp->line, cmp) == FALSE)
				return (FALSE);
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	return (TRUE);
}
