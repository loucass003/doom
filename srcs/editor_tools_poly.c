/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_tools_poly.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:33:38 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/03 14:55:11 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	print_lst(t_doom *doom, t_save *list)
{
	t_save		*tmp;
	t_line_list *cmp;

	tmp = list;
	while (tmp != NULL)
	{
		printf("FLOOR %d\n", tmp->floor);
		printf("FLOOR %d\n", doom->editor.floor);
		cmp = tmp->line;
		while (cmp != NULL)
		{
			printf("x = %f - y = %f  ||  ", cmp->line.a.x, cmp->line.a.y);
			printf("x = %f - y = %f\n\n", cmp->line.b.x, cmp->line.b.y);
			cmp = cmp->next;
		}
		tmp = tmp->next;
	}
	printf("\n\n");
}


int		ft_listlen(t_save *poly)
{
	t_save		*tmp;
	int			i;

	i = 0;
	tmp = poly;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
////////////////////////////////////////////////////////////////////////////////////

void		visual_modif(t_doom *doom, t_save *list, int x, int y)
{
	t_save		*lst;
	t_line_list *line;

	lst = list;
	while (lst && lst->floor != doom->editor.floor)
		lst = lst->next;
	line = lst->line;
	while (line)
	{
		if (doom->editor.save_modif[2] == 1)
		{
			if (doom->editor.save_modif[0] == line->line.b.x && doom->editor.save_modif[1] == line->line.b.y)
			{
				draw_line(&doom->screen, (t_pixel){line->line.a.x * 20 + 10
				, line->line.a.y * 20 + 10, 0x808080}, (t_pixel)
				{x * 20 + 10, y * 20 + 10});
			}				
			if (doom->editor.save_modif[0] == line->line.a.x && doom->editor.save_modif[1] == line->line.a.y)
			{	
				draw_line(&doom->screen, (t_pixel){x * 20 + 10
				, y * 20 + 10, 0x808080}, (t_pixel)
				{line->line.b.x * 20 + 10, line->line.b.y * 20 + 10});
			}
		}
		line = line->next;
	}
}

void		all_visual(t_doom *doom, int x, int y)
{
	if (doom->editor.lines)
		visual_modif(doom, doom->editor.lines, x, y);
	if (doom->editor.polygon)
		visual_modif(doom, doom->editor.polygon, x, y);
	if (doom->editor.door)
		visual_modif(doom, doom->editor.door, x, y);
	if (doom->editor.sector)
		visual_modif(doom, doom->editor.sector, x, y);
}

void		modify_point(t_doom *doom, t_save *liste, int x, int y)
{
	t_save		*lst;
	t_line_list *line;
	int			i;

	doom->editor.save_modif[2] = 0;
	lst = liste;
	while (lst->floor != doom->editor.floor && lst)
		lst = lst->next;
	line = lst->line;
	while (line)
	{
		i = 0;
		if (doom->editor.save_modif[0] == line->line.b.x && doom->editor.save_modif[1] == line->line.b.y)
		{
			line->line.b.x = x / 20;
			line->line.b.y = y / 20;
			i++;
			if (line->next == NULL)
			{
				doom->editor.line.b.x = x / 20;
				doom->editor.line.b.y = y / 20;
			}
		}
		if (doom->editor.save_modif[0] == line->line.a.x && doom->editor.save_modif[1] == line->line.a.y)
		{
			line->line.a.x = x / 20;
			line->line.a.y = y / 20;
			i++;
		}
		if (i == 2)
			return;
		line = line->next;
	}
}

void	modify_object(t_doom *doom, int x, int y)
{
	t_obj	*object;

	doom->editor.save_modif[2] = 0;
	object = doom->editor.obj;
	while (object) 
	{
		if (object->pos.x == doom->editor.save_modif[0] && object->pos.y == doom->editor.save_modif[1])
		{
			object->pos.x = x / 20;
			object->pos.y = y / 20;
		}
		object = object->next;
	} 
}

void	modify_all(t_doom *doom, int x, int y)
{
	if (doom->editor.polygon)
		modify_point(doom, doom->editor.polygon, x, y);
	if (doom->editor.obj)
		modify_object(doom, x, y);
	if (doom->editor.lines)
		modify_point(doom, doom->editor.lines, x, y);
	if (doom->editor.door)
		modify_point(doom, doom->editor.door, x, y);
	if (doom->editor.sector)
		modify_point(doom, doom->editor.sector, x, y);

}
void	print_line(t_doom *doom, t_line_list *list)
{
	while (list)
	{
		draw_line(&doom->screen, (t_pixel){list->line.a.x * 20 + 10
			, list->line.a.y * 20 + 10, 0xFFFFFF}, (t_pixel)
			{list->line.b.x * 20 + 10, list->line.b.y * 20 + 10});
		list = list->next;
	}
}

void	print_poly(t_doom *doom, t_save *liste, int color)
{
	t_save		*lst;
	t_line_list	*lines;

	lst = liste;
	while (lst)
	{
		if (lst->floor == doom->editor.floor)
		{
			lines = lst->line;
			while (lines)
			{
				draw_line(&doom->screen, (t_pixel){lines->line.a.x * 20 + 10
				, lines->line.a.y * 20 + 10, color}, (t_pixel)
				{lines->line.b.x * 20 + 10, lines->line.b.y * 20 + 10});
				lines = lines->next;
			}
			return;
		}
		lst = lst->next;
	}
}