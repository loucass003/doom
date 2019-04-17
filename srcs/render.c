/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/17 18:56:56 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void	render(t_doom *doom)
{

	t_node *n = doom->bsp;
	while (n->front && n->back)
	{
		
		draw_line(&doom->screen, n->partition, 0xFFFFFF);
		t_side side = get_side_thin(&n->partition, doom->player.pos);
		if (side == S_FRONT)
		{
			n = n->front;
		}
		else
			n = n->back;
	}
	doom->screen.pixels[(int)(doom->player.pos.y * doom->screen.width) + (int)doom->player.pos.x] = 0xFF00;
	draw_line(&doom->screen, (t_line){ doom->player.pos, {(int)doom->player.pos.x + (int)(5 * cosf(doom->player.rotation)), (int)doom->player.pos.y + (int)(5 * sinf(doom->player.rotation))} }, 0xFFFFFF);
	//print_node(n);
}
