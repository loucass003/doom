/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/22 18:59:31 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_vec2	it(t_vec2 a, t_vec2 b, t_vec2 c, t_vec2 d)
{
	float x = ft_vec2_cross(a, b);
	float y = ft_vec2_cross(c, d);
	float det = ft_vec2_cross(ft_vec2_sub(a, b), ft_vec2_sub(c, d));
	x = ft_vec2_cross((t_vec2){ x, a.x - b.x}, (t_vec2){y, c.x - d.x}) / det;
	y = ft_vec2_cross((t_vec2){ x, a.y - b.y}, (t_vec2){y, c.y - d.y}) / det;
	return (t_vec2){ x, y };
}

t_node	*player_node(t_doom *doom)
{
	t_node *n = doom->bsp;
	while (n->front && n->back)
	{
		t_side side = get_side_thin(&n->partition, doom->player.pos);
		if (side == S_FRONT)
		{
			n = n->front;
		}
		else
			n = n->back;
	}
	return (n);
}

#define H_FOV (0.73f * S_HEIGHT)  // Affects the horizontal field of vision
#define V_FOV (0.2f * S_HEIGHT)    // Affects the vertical field of vision

void	draw_wall(t_doom *doom, t_line *line)
{
	t_mat2 m = ft_mat2_rotation(doom->player.rotation);
	//t_vec2 ta = ft_vec2_add(ft_mat2_mulv(m, ft_vec2_sub(line->a, doom->player.pos)), (t_vec2){doom->screen.width / 2, doom->screen.height / 2});
	//t_vec2 tb = ft_vec2_add(ft_mat2_mulv(m, ft_vec2_sub(line->b, doom->player.pos)), (t_vec2){doom->screen.width / 2, doom->screen.height / 2});

	t_vec2 a = ft_vec2_sub(line->a, doom->player.pos);
	t_vec2 b = ft_vec2_sub(line->b, doom->player.pos);
	t_vec2 ta = ft_mat2_mulv(m, a);
	t_vec2 tb = ft_mat2_mulv(m, b);

	float heigthA = H_FOV / ta.y;

	float heigthB = H_FOV / tb.y;

	printf("%f %f %f %f\n", ta.x, tb.x, heigthA, heigthB);
	if (ta.y <= 0 || tb.y <= 0)
		return ;
	if (ta.y <= 0 || tb.y <= 0)
	{
		t_vec2 i1 = it(ta, tb, (t_vec2){-0.00001, 0.0001}, (t_vec2){-20, 5});
		t_vec2 i2 = it(ta, tb, (t_vec2){0.00001, 0.0001}, (t_vec2){20, 5});
		if (ta.y <= 0.00001)
			ta = i1.y > 0 ? i1 : i2;
		if (tb.y <= 0.00001)
			tb = i1.y > 0 ? i1 : i2;
	}
	int x1 = S_WIDTH_2 - ta.x * (V_FOV / ta.y);
	int x2 = S_WIDTH_2 - tb.x * (V_FOV / tb.y);
	draw_line(&doom->screen, (t_pixel){x1, S_HEIGHT_2 + (heigthA * 100) / 2, 0xFFFFFF}, (t_pixel){x2, S_HEIGHT_2 + (heigthB * 100) / 2});
	draw_line(&doom->screen, (t_pixel){x1, S_HEIGHT_2 - (heigthA * 100) / 2, 0xFFFFFF}, (t_pixel){x2, S_HEIGHT_2 - (heigthB * 100) / 2});
	draw_line(&doom->screen, (t_pixel){x1, S_HEIGHT_2 - (heigthA * 100) / 2, 0xFF0000}, (t_pixel){x1, S_HEIGHT_2 + (heigthA * 100) / 2});
	draw_line(&doom->screen, (t_pixel){x2, S_HEIGHT_2 - (heigthB * 100)/ 2, 0x00FF00}, (t_pixel){x2, S_HEIGHT_2 + (heigthB * 100) / 2});
}

void	render(t_doom *doom)
{
	t_node *n = player_node(doom);
	while(n)
	{
		
		t_side side = get_side_thin(&n->partition, doom->player.pos);
		if (side == S_FRONT)
		{
		//	draw_line(&doom->screen, (t_pixel){n->partition.a.x, n->partition.a.y, 0xFFFFFF}, (t_pixel){n->partition.b.x, n->partition.b.y});
			draw_wall(doom, &n->partition);
		}
		n = n->parent;
	}
	doom->screen.pixels[(doom->screen.height / 2 * doom->screen.width) + doom->screen.width / 2] = 0xFF00;
	draw_line(&doom->screen, (t_pixel){ doom->screen.width / 2, doom->screen.height / 2, 0xFFFFFF} , (t_pixel){doom->screen.width / 2 + (int)(5 * cosf(0)), doom->screen.height / 2 + (int)(5 * sinf(0))});

//	draw_line(&doom->screen, (t_pixel){ doom->player.pos.x, doom->player.pos.y, 0xFFFFFF} , (t_pixel){(int)doom->player.pos.x + (int)(5 * cosf(doom->player.rotation)), (int)doom->player.pos.y + (int)(5 * sinf(doom->player.rotation))});
//	doom->screen.pixels[((int)doom->player.pos.y * doom->screen.width) + (int)doom->player.pos.x] = 0xFF00;

	//print_node(n);
}
