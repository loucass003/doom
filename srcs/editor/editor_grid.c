/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_grid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Lisa <Lisa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 18:56:08 by llelievr          #+#    #+#             */
/*   Updated: 2020/04/12 18:07:27 by Lisa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"
#include "doom.h"

void	draw_grid(t_doom *doom)
{
	int	i;

	i = -1;
	while (++i <= (int)(doom->screen.height - 70) / CELLS_SPACING)
		draw_line(&doom->screen, (t_pixel){ 10, i * CELLS_SPACING + 70,
			0xFF505050}, (t_pixel){ doom->screen.width - 10,
			i * CELLS_SPACING + 70, 0xFF505050});
	i = -1;
	while (++i <= (int)(doom->screen.width - 20) / CELLS_SPACING)
		draw_line(&doom->screen, (t_pixel){ i * CELLS_SPACING + 10, 70,
			0xFF505050}, (t_pixel){ i * CELLS_SPACING + 10,
				doom->screen.height - 10, 0xFF505050});
}

void	editor_grid_render(t_doom *doom, t_editor *editor)
{
	int		color;

	draw_grid(doom);
	if (editor->grid_cell.x != -1)
	{
		color = 0xFFFF00FF;
		if (editor->grid_cell_grab == GG_POINT)
			color = 0xFFFFFF00;
		else if (editor->grid_cell_grab == GG_LINE)
			color = 0xFF00FFFF;
		draw_circle(&doom->screen, (t_pixel){ editor->grid_cell.x,
			editor->grid_cell.y, color }, 5);
	}
	if (editor->line_start_cell.x != -1 && editor->grid_cell.x != -1)
		draw_line(&doom->screen, (t_pixel){ editor->line_start_cell.x,
			editor->line_start_cell.y, 0xFF909090}, (t_pixel){
				editor->grid_cell.x, editor->grid_cell.y, 0 });
}
