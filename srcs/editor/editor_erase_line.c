/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_erase_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 13:18:39 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/25 12:12:40 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

// void	erase_obj(t_doom *doom)
// {
// 	t_obj *obj;
// 	t_obj *prev;

// 	obj = doom->editor.obj;
// 	while (obj)
// 	{
// 		prev = obj;
// 		obj = obj->next;
// 		free(prev);
// 	}
// 	doom->editor.obj = NULL;
// }

// void	erase_all_line(t_doom *doom, t_save *poly)
// {
// 	t_line_list *prev;
// 	t_line_list *list;

// 	list = poly->line;
// 	while (list)
// 	{
// 		prev = list;
// 		list = list->next;
// 		free(prev);
// 	}
// 	set_alert_message(doom);
// }

// void	erase_all_lst(t_doom *doom, t_save **liste)
// {
// 	t_save		*lst;
// 	t_save		*prev;
	
// 	lst = *liste;
// 	while (lst)
// 	{
// 		erase_all_line(doom, lst);
// 		prev = lst;
// 		lst = lst->next;
// 		free(prev);
// 	}
// 	*liste = NULL;
// }

// void	erase_all(t_doom *doom)
// {
// 	erase_all_lst(doom, &doom->editor.polygon);
// 	erase_all_lst(doom, &doom->editor.door);
// 	erase_all_lst(doom, &doom->editor.sector);
// 	erase_all_lst(doom, &doom->editor.lines);
// 	erase_obj(doom);
// 	doom->editor.click = 0;
// }

// void		mouseonline(t_doom *doom, t_line_list *tmp, int *line)
// {
// 	float			dist;
// 	float			min;
// 	float			somsega;
// 	float			somsegb;
// 	t_line_list     *list;

// 	min = 2;
// 	list = tmp;
// 	doom->linetodel.a.x = 0;
// 	doom->linetodel.b.x = 0;
// 	doom->linetodel.a.y = 0;
// 	doom->linetodel.b.y = 0;
// 	while (list)
// 	{
// 		dist = fabs(line[0] * (list->line.b.y -  list->line.a.y) 
// 		- line[1] * (list->line.b.x - list->line.a.x) 
// 		+ list->line.b.x * list->line.a.y 
// 		- list->line.b.y * list->line.a.x) 
// 		/ sqrt(pow(list->line.b.y - list->line.a.y, 2)
// 		+ pow(list->line.b.x - list->line.a.x, 2));
// 		if (dist < min)
// 		{
// 			doom->linetodel = list->line;
// 			min = dist;
// 		}
// 		else if (dist == min)
// 		{
// 			somsega = sqrt(pow(line[0] - list->line.a.x, 2) + pow(line[1] - list->line.a.y, 2)) + sqrt(pow(line[0] - list->line.b.x, 2) + pow(line[1] - list->line.b.y, 2));
// 			somsegb = sqrt(pow(line[0] - doom->linetodel.a.x, 2) + pow(line[1] - doom->linetodel.a.y, 2)) + sqrt(pow(line[0] - doom->linetodel.b.x, 2) + pow(line[1] - doom->linetodel.b.y, 2));
// 			if (somsega < somsegb)
// 				doom->linetodel = list->line;
// 		}
// 		list = list->next;
// 	}
// 	if (!tmp)
// 		doom->editor.sup = 0;
// }

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
	int index;

	printf("x %d y %d\n", x, y);
    if (doom->editor.objet)
		erase_one_obj(doom, x, y);
    if (doom->editor.polygon)
	{
		if ((index = walls_indexof(doom->editor.polygon, doom->editor.set_sup)) == -1)
			return; //TODO: ERROR
	    splice_walls_array(doom->editor.polygon, index, 1);
	}
    if (doom->editor.lines)
	{
		if ((index = walls_indexof(doom->editor.lines, doom->editor.set_sup)) == -1)
			return ;
		splice_walls_array(doom->editor.lines, index, 1);
	}
	// if (doom->editor.door)
	//     erase_line(doom, &doom->editor.door, doom->editor.set_sup);
	// if (doom->editor.sector)
	//     erase_line(doom, &doom->editor.sector, doom->editor.set_sup);

	
}
