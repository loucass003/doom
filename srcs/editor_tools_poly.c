/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools_poly.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:33:38 by lloncham          #+#    #+#             */
/*   Updated: 2019/04/26 19:04:57 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void    print_poly(t_doom *doom, t_poly *poly)
{
    t_poly *tmp1 = poly;
	t_line_list *tmp;

	while(tmp1)
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
	printf("i = %d\n", i);
	if (i > 0)
	{
		doom->editor.alert[0] = 1;
		doom->editor.click = -1;
	}
}

// t_bool	check_multi_line(t_doom *doom, t_line_list *list, int x1, int y1, int x2, int y2)
// {
// 	t_line_list *cmp = list;
// 	while (cmp)
// 	{
// 		if ((x1 == cmp->line.a.x && y1 == cmp->line.a.y && x2 == cmp->line.b.x && y2 == cmp->line.b.y) || (x1 == cmp->line.b.x && y1 == cmp->line.b.y && x2 == cmp->line.a.x && y2 == cmp->line.a.y))
// 		{
// 			doom->editor.alert[2] = 1;
// 			return FALSE;
// 		}
// 		cmp = cmp->next;
// 	}
// 	return TRUE;
// }

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
