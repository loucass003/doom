/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/17 16:49:48 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/24 01:55:57 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"
#include <assert.h>

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
#define V_FOV (0.4f * S_HEIGHT)    // Affects the vertical field of vision

void	draw_wall(t_doom *doom, t_line *line)
{
	t_vec2 a = ft_vec2_sub(line->a, doom->player.pos);
	t_vec2 b = ft_vec2_sub(line->b, doom->player.pos);
	t_vec2 ta_o = ft_mat2_mulv(doom->player.matrix, a);
	t_vec2 tb_o = ft_mat2_mulv(doom->player.matrix, b);
	t_vec2 ta = ta_o;
	t_vec2 tb = tb_o;

	if (ta.y <= 0 && tb.y <= 0)
		return ;
	if (ta.y <= 0 || tb.y <= 0)
	{
		t_vec2 i1;
		intersect(&(t_line){ta_o, tb_o}, &(t_line){{-0.1, 0.1}, (t_vec2){-20, 5}}, &i1);
		t_vec2 i2;
		intersect(&(t_line){ta_o, tb_o}, &(t_line){{0.1, 0.1}, (t_vec2){20, 5}}, &i2);
		if (ta.y < 0.1)
			ta = i1.y > 0 ? i1 : i2;
		if (tb.y < 0.1)
			tb = i1.y > 0 ? i1 : i2;
	}
	float heigthA = H_FOV / ta.y * 100;
	float heigthB = H_FOV / tb.y * 100;
	int x1 = S_WIDTH_2 - ta.x * (V_FOV / ta.y);
	int x2 = S_WIDTH_2 - tb.x * (V_FOV / tb.y);
	int x1_o = S_WIDTH_2 - ta_o.x * (V_FOV / ta_o.y);
	int x2_o = S_WIDTH_2 - tb_o.x * (V_FOV / tb_o.y);
	//int color = 0xFFFFFF / fmax(1, ft_abs(line->a.x + line->b.x + 1));
	for (int x = x1; x < x2; x++)
	{
		//printf("%d\n", (x2 - x1));
		if (x < 0 || x >= S_WIDTH || doom->rendered_area[x] != FALSE)
			continue ;
		doom->rendered_area[x] = TRUE;
		//assert((heigthB - heigthA) < 0.001);
		float h = (heigthA + (x - x1) * ((heigthB - heigthA) / (x2 - x1))) / 2.;
	/*	for (int y = fmax(0, S_HEIGHT_2 - h); y < fmin(S_HEIGHT, S_HEIGHT_2 + h); y++)
		{
			doom->screen.pixels[(y * doom->screen.width) + x] = color;
		}*/
		//draw_line(&doom->screen, (t_pixel){x,  fmax(0, S_HEIGHT_2 - (h) / 2), color}, (t_pixel){x, fmin(S_HEIGHT, S_HEIGHT_2 + (h) / 2)});
		float ax = (float)(x - x1_o) * (1. / (float)(x2_o - x1_o));
		float ux = (ax * ((float)doom->textures.bricks->w / tb_o.y)) / ((1. - ax) * (1. / ta_o.y) + ax * (1. / tb_o.y));
		apply_surface(&doom->screen, doom->textures.bricks, (SDL_Rect){ ux, 0, 1, doom->textures.bricks->h }, (SDL_Rect){ x, S_HEIGHT_2 - h, 1, h * 2 + 1});
	}
/*	draw_line(&doom->screen, (t_pixel){x1, S_HEIGHT_2 + (heigthA) / 2, 0xFFFFFF}, (t_pixel){x2, S_HEIGHT_2 + (heigthB) / 2});
	draw_line(&doom->screen, (t_pixel){x1, S_HEIGHT_2 - (heigthA) / 2, 0xFFFFFF}, (t_pixel){x2, S_HEIGHT_2 - (heigthB) / 2});
	draw_line(&doom->screen, (t_pixel){x2, S_HEIGHT_2 - (heigthB) / 2, 0x00FF00}, (t_pixel){x2, S_HEIGHT_2 + (heigthB) / 2});
	draw_line(&doom->screen, (t_pixel){x1, S_HEIGHT_2 - (heigthA) / 2, 0xFF0000}, (t_pixel){x1, S_HEIGHT_2 + (heigthA) / 2});*/
}

void visitNode(t_doom *doom, t_node *node)
{
	t_line_list *lst = node->segments;
	
	while (lst)
	{
		draw_wall(doom, &lst->line);
		lst = lst->next;
	}
}

void traverseDrawOrder(t_doom *doom, t_node *node) 
{
	t_side side;
	
	if (node) 
	{
		if (node->type == N_LEAF)
			visitNode(doom, node);
		else if ((side = get_side_thin(&node->partition, doom->player.pos)) != S_BACK) 
		{
			traverseDrawOrder(doom, node->front);
			visitNode(doom, node);
			traverseDrawOrder(doom, node->back);
		}
		else 
		{
			traverseDrawOrder(doom, node->back);
			visitNode(doom, node);
			traverseDrawOrder(doom, node->front);
		}
	}
}

void	render(t_doom *doom)
{
	ft_bzero(doom->rendered_area, doom->screen.width);
	traverseDrawOrder(doom, doom->bsp);
}
