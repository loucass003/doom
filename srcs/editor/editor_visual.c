/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_visual.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:15:26 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/24 12:33:22 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void		visual_line(t_doom *doom, t_line line)
{
	draw_line(&doom->screen, (t_pixel){line.a.x * 20 + 10
		, line.a.y * 20 + 10, 0x808080}, (t_pixel)
		{line.b.x * 20 + 10, line.b.y * 20 + 10});
}

void		visual_modif(t_doom *doom, t_walls *walls, int x, int y)
{
	// t_save		*lst;
	// t_line_list *line;

	// lst = list;
	// while (lst && lst->floor != doom->editor.floor)
	// 	lst = lst->next;
	// line = lst->line;
	// while (line)
	// {
	// 	if (doom->editor.save_modif[2] == 1)
	// 	{
	// 		if (doom->editor.save_modif[0] == line->line.b.x && doom->editor.save_modif[1] == line->line.b.y)
	// 		{
	// 			draw_line(&doom->screen, (t_pixel){line->line.a.x * 20 + 10
	// 			, line->line.a.y * 20 + 10, 0x808080}, (t_pixel)
	// 			{x * 20 + 10, y * 20 + 10});
	// 		}				
	// 		if (doom->editor.save_modif[0] == line->line.a.x && doom->editor.save_modif[1] == line->line.a.y)
	// 		{	
	// 			draw_line(&doom->screen, (t_pixel){x * 20 + 10
	// 			, y * 20 + 10, 0x808080}, (t_pixel)
	// 			{line->line.b.x * 20 + 10, line->line.b.y * 20 + 10});
	// 		}
	// 	}
	// 	line = line->next;
	// }

	int		i;
	t_wall	wall;

	i = -1;
	while (++i < walls->len)
	{
		wall = walls->values[i];
		if (doom->editor.save_modif[2] == 1)
		{
			if (doom->editor.save_modif[0] == wall.line.b.x && doom->editor.save_modif[1] == wall.line.b.y)
			{
				draw_line(&doom->screen, (t_pixel){wall.line.a.x * 20 + 10
				, wall.line.a.y * 20 + 10, 0x808080}, (t_pixel)
				{x * 20 + 10, y * 20 + 10});
			}				
			if (doom->editor.save_modif[0] == wall.line.a.x && doom->editor.save_modif[1] == wall.line.a.y)
			{	
				draw_line(&doom->screen, (t_pixel){x * 20 + 10
				, y * 20 + 10, 0x808080}, (t_pixel)
				{wall.line.b.x * 20 + 10, wall.line.b.y * 20 + 10});
			}
		}
	}
}

void		all_visual(t_doom *doom, int x, int y)
{
	if (doom->editor.lines)
		visual_modif(doom, doom->editor.lines, x, y);
	if (doom->editor.polygon)
		visual_modif(doom, doom->editor.polygon, x, y);
	// if (doom->editor.door)
	// 	visual_modif(doom, doom->editor.door, x, y);
	// if (doom->editor.sector)
	// 	visual_modif(doom, doom->editor.sector, x, y);
}
