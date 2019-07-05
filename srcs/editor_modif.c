/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_modif.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:13:41 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/05 13:32:52 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

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
	if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
		return;
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
	if (doom->editor.lines || doom->editor.door || doom->editor.sector)
	{
		if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
			return;
		if (doom->editor.lines)
			modify_point(doom, doom->editor.lines, x, y);
		if (doom->editor.door)
			modify_point(doom, doom->editor.door, x, y);
		if (doom->editor.sector)
			modify_point(doom, doom->editor.sector, x, y);
	}
}
