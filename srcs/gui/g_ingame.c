/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_ingame.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/24 11:22:28 by llelievr          #+#    #+#             */
/*   Updated: 2019/05/03 17:40:04 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

#define H_FOV (0.73f * S_HEIGHT)  // Affects the horizontal field of vision
#define V_FOV (0.4f * S_HEIGHT)    // Affects the vertical field of vision

void visitNode(t_doom *doom, t_node *node)
{
	int i = -1;
	while (++i < node->polygons->len)
	{
		t_polygon *poly = (t_polygon *)node->polygons->values[i];
		//if (poly->type == P_WALL)
			render_polygon(doom, poly);
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

		else if ((side = get_side_thin(node->partition, p)) != S_BACK) 
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

void static	action_performed(t_component *cmp, t_doom *doom)
{
	if (cmp == doom->guis[doom->current_gui].components[0])
	{
		set_gui(doom, GUI_EDITOR);
	}
}

void	g_ingame_on_enter(t_gui *self, t_doom *doom)
{

}

void	g_ingame_render(t_gui *self, t_doom *doom)
{
	ft_bzero(doom->rendered_area, doom->screen.width);
	traverseDrawOrder(doom, doom->bsp);
//	exit(0);
}
