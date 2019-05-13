/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools_poly.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:33:38 by lloncham          #+#    #+#             */
/*   Updated: 2019/05/13 16:56:07 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void    erase_line(t_doom *doom, t_poly *poly, int *line)
{
    t_poly *tmp1 = poly;
	t_line_list *tmp;
	t_line_list *prev;

	while (tmp1)
	{
		tmp = tmp1->list;
		prev = tmp;
		if (tmp->line.a.x == line[0] && tmp->line.a.y == line[1] && tmp->line.b.x == line[2] && tmp->line.b.y == line[3] && tmp)
		{
			prev = tmp->next;
			printf("line : %f - %f - %f - %f\n", prev->line.a.x, prev->line.a.y, prev->line.b.x, prev->line.b.y);
			free(tmp);
			return;
		}
		while (tmp)
    	{
			// printf("line : %f - %f - %f - %f\n", tmp->line.a.x, tmp->line.a.y, tmp->line.b.x, tmp->line.b.y);
			if (tmp->line.a.x == line[0] && tmp->line.a.y == line[1] && tmp->line.b.x == line[2] && tmp->line.b.y == line[3])
			{
				prev->next = tmp->next;
				free(tmp);
				return;
				// printf("LINE : %d - %d - %d - %d\n", line[0], line[1], line[2], line[3]);
			}
			prev = tmp;
    		tmp = tmp->next;
		}
		tmp1 = tmp1->next;
	}
}

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
			int i = -6;
			while (doom->editor.sup != 0 && i++ <= 5)
			{
				doom->screen.pixels[(((int)tmp->line.a.y * 20 + 10) + i) * doom->screen.width + ((int)tmp->line.a.x * 20) + 10] = 0xFF0000;
				doom->screen.pixels[((int)tmp->line.a.y * 20 + 10) * doom->screen.width + ((int)tmp->line.a.x * 20) + 10 + i] = 0xFF0000;
				doom->screen.pixels[(((int)tmp->line.b.y * 20 + 10) + i) * doom->screen.width + ((int)tmp->line.b.x * 20) + 10] = 0xFF0000;
				doom->screen.pixels[((int)tmp->line.b.y * 20 + 10) * doom->screen.width + ((int)tmp->line.b.x * 20) + 10 + i] = 0xFF0000;
			}
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

// t_bool    intersect2(t_line *line, t_line *seg, t_vec2 *intersect)
// {
//     if (line->a.x == seg->a.x && line->a.y == seg->a.y && line->b.x == seg->b.x && line->b.y == seg->b.y)
//         return (FALSE);
//     t_vec2 i1 = (t_vec2) {
//         line->b.y - line->a.y,
//         line->a.x - line->b.x
//     };
//     float c1 = i1.x * (line->a.x) + i1.y * (line->a.y);

//     t_vec2 i2 = (t_vec2) {
//         seg->b.y - seg->a.y,
//         seg->a.x - seg->b.x
//     };
//     float c2 = i2.x * (seg->a.x) + i2.y * (seg->a.y);
//     float d = i1.x * i2.y - i2.x * i1.y;
//     if (d == 0)
//         return (FALSE);
//     intersect->x = (i2.y * c1 - i1.y * c2) / d;
//     intersect->y = (i1.x * c2 - i2.x * c1) / d;
// 	printf("INTERSECT : 1:%f - 2:%f\n", intersect->x, intersect->y);
// 	// printf("LINE FINAL : x1:%f - x2:%f - y1:%f - y2:%f\n", line->a.x, line->b.x, line->a.y, line->b.y);
// 	// printf("X MIN :%f - MAX:%f\n", fmin(line->a.x, line->b.x), fmax(line->a.x, line->b.x));
// 	// printf("Y MIN :%f - MAX:%f\n", fmin(line->a.y, line->b.y), fmax(line->a.y, line->b.y));
//     if (intersect->x < fmax(line->a.x, line->b.x) && intersect->x > fmin(line->a.x, line->b.x) && intersect->x < fmax(seg->a.x, seg->b.x) && intersect->x > fmin(seg->a.x, seg->b.x) && 
// 	intersect->y < fmax(line->a.y, line->b.y) && intersect->y > fmin(line->a.y, line->b.y) && intersect->y < fmax(seg->a.y, seg->b.y) && intersect->y > fmin(seg->a.y, seg->b.y))
//         return (TRUE);
// 		// printf("INTER : x:%f - y:%f\n", intersect->x, intersect->y);
//     return (FALSE);
// }

t_bool	intersect0(t_line *line, t_line *cmp)
{
	float a1, a2, b1, b2;
	float x, y; 

	a1 = (line->b.y - line->a.y) / (line->b.x - line->a.x);
	a2 = (cmp->b.y - cmp->a.y) / (cmp->b.x - cmp->a.x);
	b1 = line->a.y - (a1 * line->a.x);
	b2 = cmp->a.y - (a2 * cmp->a.x);

	printf("A1 : %f - A2 : %f - B1 : %f - B2 : %f\n", a1, a2, b1, b2);
	if (a1 - a2 == 0)
		return TRUE; //ne croisent pas
	if (a2 == 0) // droite horizontale + verticale a faire !!!
	{
		float x = cmp->a.x;
		if ((line->a.x < x && line->b.x > x) || (line->a.x > x && line->b.x < x))
			return FALSE;
	}
	if (a1 == 0)
	{
		float x = line->a.x;
		if ((cmp->a.x < x && cmp->b.x > x) || (cmp->a.x > x && cmp->b.x < x))
			return FALSE;
	}
	x = (b2 - b1) / (a1 - a2);
	y = a1 * x + b1;
// 	printf("INTERSECT 1 %f\n", x);
	if (x < fmax(line->a.x, line->b.x) && x > fmin(line->a.x, line->b.x) && x < fmax(cmp->a.x, cmp->b.x) && x > fmin(cmp->a.x, cmp->b.x) &&
		y < fmax(line->a.y, line->b.y) && y > fmin(line->a.y, line->b.y) && y < fmax(cmp->a.y, cmp->b.y) && y > fmin(cmp->a.y, cmp->b.y))
			return (FALSE);
	else
		return TRUE;
// 	// if (line->b.y == line->a.y)
// 	// {
// 	// 	if (cmp->a.y == cmp->b.y)
// 	// 	{
// 	// 		if (x < fmax(line->a.x, line->b.x) && x > fmin(line->a.x, line->b.x) && x < fmax(cmp->a.x, cmp->b.x) && x > fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y <= fmax(line->a.y, line->b.y) && y >= fmin(line->a.y, line->b.y) && y <= fmax(cmp->a.y, cmp->b.y) && y >= fmin(cmp->a.y, cmp->b.y))
// 	// 			return (FALSE);
// 	// 	}
// 	// 	else if (cmp->a.x == cmp->b.x)
// 	// 	{
// 	// 		if (x < fmax(line->a.x, line->b.x) && x > fmin(line->a.x, line->b.x) && x <= fmax(cmp->a.x, cmp->b.x) && x >= fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y <= fmax(line->a.y, line->b.y) && y >= fmin(line->a.y, line->b.y) && y < fmax(cmp->a.y, cmp->b.y) && y > fmin(cmp->a.y, cmp->b.y))
// 	// 			return (FALSE);
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		if (x < fmax(line->a.x, line->b.x) && x > fmin(line->a.x, line->b.x) && x < fmax(cmp->a.x, cmp->b.x) && x > fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y <= fmax(line->a.y, line->b.y) && y >= fmin(line->a.y, line->b.y) && y < fmax(cmp->a.y, cmp->b.y) && y > fmin(cmp->a.y, cmp->b.y))
// 	// 			return (FALSE);
// 	// 	}	
// 	// }
// 	// else if (line->b.x == line->a.x)
// 	// {
// 	// 	if (cmp->a.y == cmp->b.y)
// 	// 	{
// 	// 		if (x <= fmax(line->a.x, line->b.x) && x >= fmin(line->a.x, line->b.x) && x < fmax(cmp->a.x, cmp->b.x) && x > fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y < fmax(line->a.y, line->b.y) && y > fmin(line->a.y, line->b.y) && y <= fmax(cmp->a.y, cmp->b.y) && y >= fmin(cmp->a.y, cmp->b.y))
// 	// 			return (FALSE);
// 	// 	}
// 	// 	else if (cmp->a.x == cmp->b.x)
// 	// 	{
// 	// 		if (x <= fmax(line->a.x, line->b.x) && x >= fmin(line->a.x, line->b.x) && x <= fmax(cmp->a.x, cmp->b.x) && x >= fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y < fmax(line->a.y, line->b.y) && y > fmin(line->a.y, line->b.y) && y < fmax(cmp->a.y, cmp->b.y) && y > fmin(cmp->a.y, cmp->b.y))
// 	// 			return (FALSE);
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		if (x <= fmax(line->a.x, line->b.x) && x >= fmin(line->a.x, line->b.x) && x < fmax(cmp->a.x, cmp->b.x) && x > fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y < fmax(line->a.y, line->b.y) && y > fmin(line->a.y, line->b.y) && y < fmax(cmp->a.y, cmp->b.y) && y > fmin(cmp->a.y, cmp->b.y))
// 	// 			return (FALSE);
// 	// 	}	
// 	// }
// 	// else if (x < fmax(line->a.x, line->b.x) && x > fmin(line->a.x, line->b.x) && x < fmax(cmp->a.x, cmp->b.x) && x > fmin(cmp->a.x, cmp->b.x) &&
// 	// 				y < fmax(line->a.y, line->b.y) && y > fmin(line->a.y, line->b.y) && y < fmax(cmp->a.y, cmp->b.y) && y > fmin(cmp->a.y, cmp->b.y))
// 	// 				return (FALSE);
}

t_bool	check_secant_line(t_doom *doom, t_poly *poly, float x1, float x2, float y1, float y2)
{
	t_poly *tmp = poly;
	t_vec2 *inter;
	t_line_list *cmp;
	t_line	line;

	line.a.x = x1;
	line.b.x = x2;
	line.a.y = y1;
	line.b.y = y2;
	printf("LINE : x1:%f - x2:%f - y1:%f - y2:%f\n", line.a.x, line.b.x, line.a.y, line.b.y);
	while (tmp)
	{
		cmp = poly->list;
		while (cmp)
		{
			if (line.a.x == cmp->line.a.x && line.a.y == cmp->line.a.y && line.b.x == cmp->line.b.x && line.b.y == cmp->line.b.y)
				cmp = cmp->next;
			printf("CMP : x1:%f - x2:%f - y1:%f - y2:%f\n", cmp->line.a.x, cmp->line.b.x, cmp->line.a.y, cmp->line.b.y);
    	    if (intersect0(&line, &cmp->line) == FALSE)
				return FALSE;
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	printf("TRUE\n");
	return (TRUE);
}