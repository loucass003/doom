/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_erase_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: louali <louali@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:18:39 by lloncham          #+#    #+#             */
/*   Updated: 2019/09/17 14:32:45 by louali           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	erase_all(t_doom *doom)
{
	splice_walls_array(doom->editor.lines, 0, doom->editor.lines->len);
	splice_walls_array(doom->editor.polygon, 0, doom->editor.polygon->len);
	splice_objects_array(doom->editor.objects, 0, doom->editor.objects->len);
	doom->editor.alert[0] = 0;
	doom->editor.click = 0;
}

void		mouseonline(t_doom *doom, t_walls *tmp, int *line)
{
	float			dist;
	float			min;
	float			somsega;
	float			somsegb;
	// t_line_list     *list;
	t_walls		     *list;

	int		index;

	index = -1;
	min = 2;
	list = tmp;
	doom->linetodel.line.a.x = 0;
	doom->linetodel.line.b.x = 0;
	doom->linetodel.line.a.y = 0;
	doom->linetodel.line.b.y = 0;
	while (++index < list->len)
	{
		dist = fabs(line[0] * (list->values[index].line.b.y -  list->values[index].line.a.y) 
		- line[1] * (list->values[index].line.b.x - list->values[index].line.a.x) 
		+ list->values[index].line.b.x * list->values[index].line.a.y 
		- list->values[index].line.b.y * list->values[index].line.a.x) 
		/ sqrt(pow(list->values[index].line.b.y - list->values[index].line.a.y, 2)
		+ pow(list->values[index].line.b.x - list->values[index].line.a.x, 2));
		if (dist < min)
		{
			doom->linetodel.img = list->values[index].img;
			doom->linetodel.line = list->values[index].line;
							doom->indextodel = index;
				if (tmp == doom->editor.polygon)
					doom->wheredel = 1;
				else if (tmp == doom->editor.lines)
					doom->wheredel = 0;
			min = dist;
		}
		else if (dist == min)
		{
			somsega = sqrt(pow(line[0] - list->values[index].line.a.x, 2) + pow(line[1] - list->values[index].line.a.y, 2)) + sqrt(pow(line[0] - list->values[index].line.b.x, 2) + pow(line[1] - list->values[index].line.b.y, 2));
			somsegb = sqrt(pow(line[0] - doom->linetodel.line.a.x, 2) + pow(line[1] - doom->linetodel.line.a.y, 2)) + sqrt(pow(line[0] - doom->linetodel.line.b.x, 2) + pow(line[1] - doom->linetodel.line.b.y, 2));
			if (somsega < somsegb)
			{
				doom->linetodel.img = list->values[index].img;
				doom->linetodel.line = list->values[index].line;
				doom->indextodel = index;
				if (tmp == doom->editor.polygon)
					doom->wheredel = 1;
				else if (tmp == doom->editor.lines)
					doom->wheredel = 0;
			}
		}
	}
	// printf("%f\n %f\n", doom->linetodel->line.a.x, doom->linetodel->line.a.y);
	if (!tmp)
		doom->editor.sup = 0;
}

// void	erase_one_line(t_doom *doom, t_save *tmp1, int *line)
// {
// 	t_line_list *prev;
// 	t_line_list *tmp;

// 	tmp = tmp1->line;
// 	if (((tmp->line.a.x == doom->linetodel.a.x && tmp->line.a.y == doom->linetodel.a.y
// 	&& tmp->line.b.x == doom->linetodel.b.x && tmp->line.b.y == doom->linetodel.b.y)
// 	|| (tmp->line.a.x == doom->linetodel.b.x && tmp->line.a.y == doom->linetodel.b.y
// 	&& tmp->line.b.x == doom->linetodel.a.x && tmp->line.b.y == doom->linetodel.a.y)) && tmp)
// 	{
// 		tmp1->line = tmp->next;
// 		free(tmp);
// 		tmp = NULL;
// 		doom->editor.click = 1;
//         check_poly_close(doom, tmp1);
// 		return ;
// 	}
// 	prev = tmp;
// 	while (tmp)
// 	{
// 		if ((tmp->line.a.x == doom->linetodel.a.x && tmp->line.a.y == doom->linetodel.a.y
// 	&& tmp->line.b.x == doom->linetodel.b.x && tmp->line.b.y == doom->linetodel.b.y)
// 	|| (tmp->line.a.x == doom->linetodel.b.x && tmp->line.a.y == doom->linetodel.b.y
// 	&& tmp->line.b.x == doom->linetodel.a.x && tmp->line.b.y == doom->linetodel.a.y))
// 		{
// 			prev->next = tmp->next;
// 			free(tmp);
// 			tmp = NULL;
// 			doom->editor.click = 1;
//         	check_poly_close(doom, tmp1);
// 			return ;
// 		}
// 		prev = tmp;
// 		tmp = tmp->next;
// 	}

// }

// void	erase_line(t_doom *doom, t_save **list, int *line)
// {
// 	t_save		*lst;
// 	t_save		*prev1;

// 	lst = *list;
// 	prev1 = lst;
// 	while (lst->floor != doom->editor.floor && lst)
// 	{
// 		prev1 = lst;
// 		lst = lst->next;
//     }
// 	erase_one_line(doom, lst, line);
// 	if (lst->line == NULL && lst == *list)
// 	{
// 	    *list = lst->next;
// 		free(lst);
// 		lst = NULL;
// 		doom->editor.click = 0;
// 		return ;
// 	}
// 	else if (lst->line == NULL)
// 	{
// 		prev1->next = lst->next;
// 		doom->editor.click = 0;
// 		free(lst);
// 		lst = NULL;
// 		return ;
//     }
// }

void	erase_one_obj(t_doom *doom, int x, int y) // pourquoi ne coupe pas le dernier??
{
	int		index;

	index = -1;
	while (++index < doom->editor.objects->len)
		if (doom->editor.objects->values[index].pos.x == x && doom->editor.objects->values[index].pos.y == y)
			break;
	if (index == doom->editor.objects->len)
		return ;
	splice_objects_array(doom->editor.objects, index, 1);
}

void	save_line_to_erase(t_doom *doom, int x, int y)
{
    if (doom->editor.objet)
			erase_one_obj(doom, x, y);
    if (doom->editor.polygon && doom->wheredel == 1)
	{
		doom->editor.alert[0] = 0;
		splice_walls_array(doom->editor.polygon, doom->indextodel, 1);
	}
	else if (doom->wheredel == 0 && doom->editor.lines)
			splice_walls_array(doom->editor.lines, doom->indextodel, 1);
	// if (doom->editor.door)
	    // erase_line(doom, &doom->editor.door, doom->editor.set_sup);
	// if (doom->editor.sector)
	    // erase_line(doom, &doom->editor.sector, doom->editor.set_sup);
}
