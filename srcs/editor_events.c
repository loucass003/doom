/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 17:31:07 by lloncham          #+#    #+#             */
/*   Updated: 2019/04/26 19:04:42 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	editor_mouse_motion(t_doom *doom, SDL_Event *event)
{
	int x;
	int y;
	unsigned int index;

	x = event->motion.x;
    y = event->motion.y;
	if (x < (doom->screen.width - 180))
	{
		x /= 20;
		y /= 20;
		if (doom->editor.point[(y *  doom->screen.width / 20 ) + x] == 0)
		{
			ft_bzero(doom->editor.point, sizeof(uint8_t) * (((doom->screen.width - 200) * doom->screen.height) / 20));
			doom->editor.point[(y *  doom->screen.width / 20 ) + x] = 1;
		}
	}
}

void	editor_mousebuttonup(t_doom *doom, SDL_Event *event)
{
	if (doom->editor.click == 0)
	{
		doom->editor.line.a.x = (int)event->button.x / 20;
		doom->editor.line.a.y = (int)event->button.y / 20;
		if (doom->editor.first[0] == 0 && doom->editor.first[1] == 0)
		{
			doom->editor.first[0] = doom->editor.line.a.x;
			doom->editor.first[1] = doom->editor.line.a.y;
		}
	}
	else if (check_multi_point(doom, doom->editor.list, (int)event->button.x / 20, (int)event->button.y / 20) == TRUE)
	{
		doom->editor.alert[1] = 0;
		doom->editor.alert[2] = 0;
		doom->editor.alert[0] = 0;

		if (doom->editor.click > 1)
		{
			doom->editor.line.a.x = doom->editor.line.b.x;
			doom->editor.line.a.y = doom->editor.line.b.y;
		}
		if (check_multi_line(doom, doom->editor.list, doom->editor.line.a.x, doom->editor.line.a.y, (int)event->button.x / 20, (int)event->button.y / 20) == FALSE)
			return;
		doom->editor.line.b.x = (int)event->button.x / 20;
		doom->editor.line.b.y = (int)event->button.y / 20;
		if (doom->editor.line.b.x == doom->editor.line.a.x && doom->editor.line.a.y == doom->editor.line.b.y)
		{
			doom->editor.alert[1] = 1;
			return;
		}
		append_list(&doom->editor.list, doom->editor.line);
		check_poly_close(doom, doom->editor.list);
	}
	doom->editor.click++;
}