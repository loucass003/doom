/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_modif.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:13:41 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/25 10:58:59 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void		modify_point(t_doom *doom, t_walls *walls, int x, int y)
{
	int			i;
	int			j;
	t_line		*line;

	j = -1;
	doom->editor.save_modif[2] = 0;
	while (++j < walls->len)
	{
		i = 0;
		line = &walls->values[j].line;
		if (doom->editor.save_modif[0] == line->b.x && doom->editor.save_modif[1] == line->b.y)
		{
			line->b.x = x / 20;
			line->b.y = y / 20;
			i++;
		}
		if (doom->editor.save_modif[0] == line->a.x && doom->editor.save_modif[1] == line->a.y)
		{
			line->a.x = x / 20;
			line->a.y = y / 20;
			i++;
		}
		if (i == 2)
			return;
	}
}

void	modify_object(t_doom *doom, int x, int y)
{
	t_object	*object;
	int			i;

	doom->editor.save_modif[2] = 0;
	if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
		return;
	i = -1;
	while (++i < doom->editor.objects->len)
	{
		object = &doom->editor.objects->values[i];
		if (object->pos.x == doom->editor.save_modif[0] && object->pos.y == doom->editor.save_modif[1])
		{
			object->pos.x = x / 20;
			object->pos.y = y / 20;
		}
	}
}

void	modify_all(t_doom *doom, int x, int y)
{
	if (doom->editor.polygon)
		modify_point(doom, doom->editor.polygon, x, y);
	if (doom->editor.objects)
		modify_object(doom, x, y);
	if (doom->editor.lines /*|| doom->editor.door || doom->editor.sector*/)
	{
		if (in_the_poly(doom, doom->editor.polygon, (t_vec2){x / 20, y / 20}) == FALSE)
			return;
		if (doom->editor.lines)
			modify_point(doom, doom->editor.lines, x, y);
		// if (doom->editor.door)
		// 	modify_point(doom, doom->editor.door, x, y);
		// if (doom->editor.sector)
		// 	modify_point(doom, doom->editor.sector, x, y);
	}
}
