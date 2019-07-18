/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/07/18 03:44:21 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

int g_count = 0;

void visitNode(t_doom *doom, t_node *node)
{
/*	if (!node->polygons || node->polygons->len == 0)
		return ;*/
	g_count++;
	int i = -1;
	while (++i < node->polygons->len)
	{
		t_polygon *poly = &node->polygons->polygons[i];
		//if (poly->type == P_WALL)
		render_polygon(doom, poly);
		if (node->parent && node->parent->front == node)
		draw_obb(doom, poly->obb);
	//	printf("draw %s\n", ((t_polygon *)node->polygons->values[i])->type == P_WALL ? "WALL" : "FLOOR");
	}
	/*t_line_list *lst = node->segments;
	//if (node->type == N_LEAF)
//	printf("VISIT %p\n", node);
	while (lst)
	{
		if (lst->line.type == L_WALL)
			draw_wall(doom, &lst->line);
		else
			draw_floor_seg(doom, &lst->line, node);
		lst = lst->next;
	}*/
}

void traverseDrawOrder(t_doom *doom, t_node *node) 
{
	t_side side;
	
	if (node) 
	{
		t_vec2 p = (t_vec2){doom->player.pos.x, doom->player.pos.z};
		if (node->type == N_LEAF)
			visitNode(doom, node);
		else if (get_side_thin(node->partition, p) != S_BACK) 
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

int x = 30;

void trasverseInOrder(t_doom *doom, t_node *node)
{
	if (node->front)
		trasverseInOrder(doom, node->front);
	node->display_pos = (t_vec2) { x, 30 * (node->depth + 1) };
	x += 30;
	if (node->back)
		trasverseInOrder(doom, node->back);
}

void draw_graph(t_doom *doom, t_node *node)
{
	if (node->front)
		draw_graph(doom, node->front);
	if (node->back)
		draw_graph(doom, node->back);
	t_pixel p1 = (t_pixel) {node->display_pos.x, node->display_pos.y, 0xFFFFFF};
	if (node->parent)
	{
		t_pixel p0 = (t_pixel) {node->parent->display_pos.x, node->parent->display_pos.y, 0xFFFFFF};
		draw_line(&doom->screen, p0, p1);
	}
	p1 = (t_pixel) {node->display_pos.x, node->display_pos.y, doom->player.curr_node == node ? 0xFF00FF : 0xFF};
	draw_circle(&doom->screen, p1, 15);
}

void static	action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		set_gui(doom, GUI_EDITOR);
	}
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{
	trasverseInOrder(doom, doom->bsp);
}

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	for (int i = 0; i < doom->screen.width * doom->screen.height; i++)
		doom->rendered_area[i] = 0;
	g_count = 0;
	traverseDrawOrder(doom, doom->bsp);
	doom->obj->position = (t_vec3){2, 0, 2};
	//doom->obj->rotation.y += 3 * doom->stats.delta;
	doom->obj->scale = (t_vec3){0.002, 0.002, 0.002};
	//doom->obj->dirty = TRUE;
	render_obj(doom, doom->obj);

		
	//printf("%d\n", g_count);
	draw_graph(doom, doom->bsp);
	//exit(0);
}
