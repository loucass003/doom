/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/16 19:44:41 by llelievr          #+#    #+#             */
/*   Updated: 2019/04/18 16:34:15 by llelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

t_bool	append_list(t_line_list **list, t_line line)
{
	t_line_list	*n;
	t_line_list	*elem;

	if (!(elem = (t_line_list *)malloc(sizeof(t_line_list))))
		return (FALSE);
	elem->line = line;
	elem->next = NULL;
	if (!*list)
		*list = elem;
	else
	{
		n = *list;
		while (n && n->next)
			n = n->next;
		n->next = elem;
	}
	return (TRUE);
}

t_node	*create_node(t_line_list *segments)
{
	t_node *node;

	if (!(node = (t_node *)malloc(sizeof(t_node))))
		return (NULL);
	node->front = NULL;
	node->back = NULL;
	node->segments = segments;
	node->parent = NULL;
	node->type = N_NODE;
	if (segments)
		node->partition = segments->line;
	else
	{
		node->type = N_LEAF;
		//TODO: add more data (floor_height, ceil_height)
	}
	return (node);
}

void	build_node(t_node *node)
{
	t_line_list	*lst;
	t_line_list	*front;
	t_line_list	*back;
	t_line_list	*colinear;

	if (node->type == N_LEAF)
		return ;
	lst = node->segments;
	front = NULL;
	back = NULL;
	colinear = NULL;
	while (lst)
	{
		t_side side = get_side(&node->partition, lst->line);
		if (side == S_COLINEAR)
			append_list(&colinear, lst->line);
		else if (side == S_FRONT)
			append_list(&front, lst->line);
		else if (side == S_BACK)
			append_list(&back, lst->line);
		else if (side == S_SPANNING)
		{
			t_vec2 it;
			if (!intersect(&node->partition, &lst->line, &it))
			{
				lst = lst->next;
				continue;
			}
			t_line a = { lst->line.a, it };
			t_line b = { it, lst->line.b };
			t_side side = get_side(&node->partition, a);
			if (side == S_FRONT)
				append_list(&front, a);
			else if (side == S_BACK)
				append_list(&back, a);
			side = get_side(&node->partition, b);
			if (side == S_FRONT)
				append_list(&front, b);
			else if (side == S_BACK)
				append_list(&back, b);
		}
		lst = lst->next;
	}
	node->segments = colinear;
	node->front = create_node(front);
	node->front->parent = node;
	node->back = create_node(back);
	node->back->parent = node;
	build_node(node->front);
	build_node(node->back);
}

void	print_node(t_node *n)
{
	if (!n)
		return ;
	if (n->front)
		printf("N%p ==> N%p\n", n, n->front);
	if (n->back)
		printf("N%p --> N%p\n", n, n->back);
	if (n->type == N_NODE)
	{
		printf("N%p(%s - %.3f, %.3f - %.3f, %.3f)\n", n,
			n->type == N_LEAF ? "LEAF" : "NODE",
			n->partition.a.x,
			n->partition.a.y,
			n->partition.b.x,
			n->partition.b.y);
	}
	else
	{
		printf("N%p[LEAF]\n", n);
	}
	print_node(n->front);
	print_node(n->back);
}
