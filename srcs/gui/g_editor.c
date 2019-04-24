/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_editor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lloncham <lloncham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 15:50:09 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 18:13:47 by lloncham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

static void		action_performed(t_component *cmp, t_doom *doom)
{
	
}

void	g_editor_on_enter(t_gui *self, t_doom *doom)
{
	printf("ENTER\n");
	if (!(doom->editor.point = (uint8_t *)malloc(sizeof(uint8_t) * (doom->screen.size / 20))))
		return ; //TODO: ERROR
	ft_bzero(doom->editor.point, sizeof(uint8_t) * (doom->screen.size / 20));
}

void	g_editor_on_leave(t_gui *self, t_doom *doom)
{
	free(doom->editor.point);
}

void	g_editor_on_event(t_gui *self, SDL_Event *event, t_doom *doom)
{
	int x;
	int y;
	unsigned int index;

	if (event->type == SDL_MOUSEMOTION)
	{
		
		x = event->motion.x;
        y = event->motion.y;

		/*if (true || x > 201)
		{*/
			x = (x/20);
			y = (y/20);
			/*if ((index = y * doom->screen.width + x) <= doom->screen.size)
            	doom->screen.pixels[index] = 0x00FF0000;*/
			if (doom->editor.point[(y *  doom->screen.width / 20 ) + x] == 0)
			{
				ft_bzero(doom->editor.point, sizeof(uint8_t) * (doom->screen.size / 20));
				doom->editor.point[(y *  doom->screen.width / 20 ) + x] = 1;
			}
		/*}*/

	}
}

void	g_editor_render(t_gui *self, t_doom *doom)
{
	int x;
	int y;

	y = 0;
	while (y <= doom->screen.height / 20)
	{
		x = 0;
		while (x <= doom->screen.width / 20)
		{
			doom->screen.pixels[(y * 20 + 10) * doom->screen.width + (x * 20) + 10] = doom->editor.point[(y *  (doom->screen.width / 20) ) + x] == 1 ? 0xFF0000 : 0xFFFFFF;
			x++;
		}
		y ++;
	}
	//fill_color(&doom->screen, 0xFF0000);
}
