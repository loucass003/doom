/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_check_valid_poly.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 14:58:57 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/31 15:59:46 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool	check_obj_legit(t_doom *doom, int x, int y)
{
	int			i;
	t_object	object;

	i = -1;
	while (++i < doom->editor.objects->len)
	{
		object = doom->editor.objects->values[i];
		if (x == (int)object.pos.x && y == (int)object.pos.y)
			return (FALSE);
		if (x == doom->editor.set_start_pos[0] && y == doom->editor.set_start_pos[1] && doom->editor.set_start_pos[0])
			return (FALSE);
	}
	return (TRUE);
}

void	check_poly_close(t_doom *doom, t_walls *walls)
{
	int		i;
	t_line	n;
	t_line	n_next;

	i = -1;
	while (++i < walls->len)
	{
		n = walls->values[i].line;
		n_next = walls->values[(i + 1) % walls->len].line;
		if (n.b.x != n_next.a.x || n.b.y != n_next.a.y)
		{
			doom->editor.alert[0] = 0;
			return;
		}
	}
	doom->editor.alert[0] = 1;
	doom->editor.click = -1;
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

// // t_bool	check_multi_point(t_doom *doom, t_poly *poly, int x, int y)
// // {
// // 	t_poly		*polygons;
// // 	t_line_list *list;
// // 	int			i;

// // 	polygons = poly;
// // 	while (polygons)
// // 	{
// // 		list = polygons->list;
// // 		i = 0;
// // 		while (list)
// // 		{
// // 			if (list->floor == doom->editor.floor)
// // 			{
// // 				if ((x == list->line.b.x && y == list->line.b.y)
// // 				|| (x == list->line.a.x && y == list->line.a.y))
// // 					i++;
// // 				if (i > 1)
// // 				{
// // 					doom->editor.alert[1] = 1;
// // 					return (FALSE);
// // 				}
// // 			}
// // 			list = list->next;
// // 		}
// // 		polygons = polygons->next;
// // 	}
// // 	return (TRUE);
// // }
// //////////////////////////////////////////////////////
t_bool	found_intersect(t_line *line, t_line *cmp, t_walls *walls, int start)
{
	float		a1;
	float		a2;
	float		b1;
	float		b2;
	float		x;
	float		y;

	if (line->b.x == line->a.x || cmp->b.x == cmp->a.x)
	{
		if (line->b.x == line->a.x)
		{
			a2 = (cmp->b.y - cmp->a.y) / (cmp->b.x - cmp->a.x);
			b2 = cmp->a.y - (a2 * cmp->a.x);
			x = line->b.x;
			y = a2 * x + b2;
		}
		else
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
		while (start < walls->len)
		{
			t_line	c_line = walls->values[start++].line;
			x += 0.01;
			y += 0.01;
			x = (int)x;
			y = (int)y;
			if ((c_line.a.x == x && c_line.a.y == y && x == line->a.x && y == line->a.y)
			|| (c_line.a.x == x && c_line.a.y == y && x == line->b.x && y == line->b.y)
			|| (c_line.b.x == x && c_line.b.y == y && x == line->b.x && y == line->b.y)
			|| (c_line.b.x == x && c_line.b.y == y && x == line->a.x && y == line->a.y))
			{
				return (TRUE);
			}
		}
		return (FALSE);
	}
	return (TRUE);
}

t_bool	check_secant_line(t_doom *doom, t_walls *walls, t_line line)
{
	// t_save		*lst;
	// t_line_list	*lines;

	// lst = list;
	// if (!list)
	// 	return (TRUE);
	// while (lst->floor != doom->editor.floor && lst)
	// 	lst = lst->next;
	// lines = lst->line;
	// while (lines)
	// {
	// 	if (found_intersect(&line, &lines->line, lines) == FALSE) //savoir si deux segments se croisent
	// 		return (FALSE);
	// 	lines = lines->next;
	// }
	// return (TRUE);
	int			i;

	i = -1;
	while (++i < walls->len)
	{
		if (found_intersect(&line, &walls->values[i].line, walls, i) == FALSE)
			return (FALSE);
	}
	return (TRUE);
}

// // t_bool	check_multi_line(t_doom *doom, t_save *list, t_line line)
// // {
// // 	t_save		*lst;
// // 	t_line_list *poly;

// // 	lst = list;
// // 	printf("coucou \n");
// // 	printf("coucou %d\n", lst->floor);
// // 	while (lst->floor != doom->editor.floor && lst)
// // 		lst = lst->next;
// // 	poly = lst->line; 
// // 	while (poly)
// // 	{
// // 		if ((line.a.x == poly->line.a.x && line.a.y == poly->line.a.y
// // 		&& line.b.x == poly->line.b.x && line.b.y == poly->line.b.y)
// // 		|| (line.a.x == poly->line.b.x && line.a.y == poly->line.b.y
// // 		&& line.b.x == poly->line.a.x && line.b.y == poly->line.a.y))
// // 		{
// // 			doom->editor.alert[2] = 1;
// // 			return (FALSE);
// // 		}
// // 		list = list->next;
// // 	}
// // 	return (TRUE);
// // }
