/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools_poly.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:33:38 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/06 16:09:28 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void    print_poly(t_doom *doom, t_poly *poly)
{
    t_poly *tmp1 = poly;
	t_line_list *tmp;

	while (tmp1)
	{
		tmp = tmp1->list;
    	while (tmp != NULL)
    	{
    	    draw_line(&doom->screen, (t_pixel){tmp->line.a.x * 20 + 10, tmp->line.a.y * 20 + 10, 0xFFFFFF}, (t_pixel){tmp->line.b.x * 20 + 10, tmp->line.b.y * 20 + 10});
    	    tmp = tmp->next;
    	}
		tmp1 = tmp1->next;
	}
}

void	check_poly_close(t_doom *doom, t_poly *poly)
{
	t_line_list *cmp;
	t_poly *tmp = poly;
	int i = 0;

	while (tmp)
	{
		i = 0;
		cmp = tmp->list;
		while (cmp)
		{
			if ((cmp->line.b.x == doom->editor.first[0] && cmp->line.b.y == doom->editor.first[1]) || (cmp->line.b.x == doom->editor.first[0] && cmp->line.b.y == doom->editor.first[1]))
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
	t_poly	*tmp = poly;
	t_line_list *cmp;
	while (tmp)
	{
		cmp = tmp->list;
		while (cmp)
		{
			if ((x1 == cmp->line.a.x && y1 == cmp->line.a.y && x2 == cmp->line.b.x && y2 == cmp->line.b.y) || (x1 == cmp->line.b.x && y1 == cmp->line.b.y && x2 == cmp->line.a.x && y2 == cmp->line.a.y))
			{
				doom->editor.alert[2] = 1;
				return FALSE;
			}
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	return TRUE;
}

t_bool	check_same_point(t_doom *doom)
{
	if (doom->editor.line.b.x == doom->editor.line.a.x && doom->editor.line.a.y == doom->editor.line.b.y)
	{
		doom->editor.alert[1] = 1;
		return FALSE;
	}
	return TRUE;
}

t_bool	check_multi_point(t_doom *doom, t_poly *poly, int x, int y)
{
	t_poly *tmp = poly;
	t_line_list *cmp;

	while (tmp)
	{
		cmp = tmp->list;
		int i = 0;
		while (cmp)
		{
			if ((x == cmp->line.b.x && y == cmp->line.b.y) || (x == cmp->line.a.x && y == cmp->line.a.y))
				i++;
			if (i > 1)
			{
				doom->editor.alert[1] = 1;
				return FALSE;
			}
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	return TRUE;
}

t_bool	check_line(t_img *img, t_pixel p0, t_pixel p1)
{
	t_pixel			d;
	t_pixel			s;
	int				e[2];
	unsigned int	index;

	d = (t_pixel){ ft_abs(p1.x - p0.x), ft_abs(p1.y - p0.y), 0 };
	s = (t_pixel){ (p0.x < p1.x ? 1 : -1), (p0.y < p1.y ? 1 : -1), 0 };
	e[0] = (d.x > d.y ? d.x : -d.y) / 2;
	while (p0.x != p1.x || p0.y != p1.y)
	{
		if (p0.x >= 0 && p0.x < img->width && p0.y >= 0 && p0.y < img->height)
			if (img->pixels[index = p0.y * img->width + p0.x] == p0.color)
				return FALSE;
		e[1] = e[0];
		if (e[1] > -d.x)
		{
			e[0] -= d.y;
			p0.x += s.x;
		}
		if (e[1] < d.y)
		{
			e[0] += d.x;
			p0.y += s.y;
		}
	}
	return TRUE;
}

t_bool	check_secant_line(t_doom *doom, t_poly *poly, int x1, int x2, int y1, int y2)
{
	t_poly *tmp = poly;
	t_vec2 *inter;
	t_line_list *cmp;
	t_line	line;

	line.a.x = x1;
	line.b.x = x2;
	line.a.y = y1;
	line.b.y = y2;
	while (tmp)
	{
		cmp = tmp->list;
		while (cmp)
		{
			if (line.a.x == cmp->line.a.x && line.a.y == cmp->line.a.y && line.b.x == cmp->line.b.x && line.b.y == cmp->line.b.y)
				cmp = cmp->next;
    	    if (check_line(&doom->screen, (t_pixel){cmp->line.a.x * 20 + 10, cmp->line.a.y * 20 + 10, 0xFFFFFF}, (t_pixel){cmp->line.b.x * 20 + 10, cmp->line.b.y * 20 + 10}) == FALSE);
				return FALSE;
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	return (TRUE);
}