/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:14:43 by lloncham          #+#    #+#             */
/*   Updated: 2019/08/28 14:02:10 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	print_poly(t_doom *doom, t_walls *walls, int color)
{
	int		i;
	t_wall	wall;

	i = -1;
	while (++i < walls->len)
	{
		wall = walls->values[i];
		draw_line(&doom->screen, (t_pixel){wall.line.a.x * 20 + 10
				, wall.line.a.y * 20 + 10, color}, (t_pixel)
				{wall.line.b.x * 20 + 10, wall.line.b.y * 20 + 10});
	}
}

void	print_all(t_doom *doom)
{
	print_poly(doom, doom->editor.polygon, 0xFF00FFFF);
	print_poly(doom, doom->editor.lines, 0xFFFF0000); 
	// print_poly(doom, doom->editor.sector, 0xFF00FF);
	// print_poly(doom, doom->editor.door, 0x800080);
}
