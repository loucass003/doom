/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 17:14:43 by lloncham          #+#    #+#             */
/*   Updated: 2019/07/03 17:17:19 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

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

void	print_all(t_doom *doom)
{
	print_poly(doom, doom->editor.polygon, 0x00FFFF);
	print_poly(doom, doom->editor.lines, 0xFF0000); 
	print_poly(doom, doom->editor.sector, 0xFF00FF);
	print_poly(doom, doom->editor.door, 0x800080);
}